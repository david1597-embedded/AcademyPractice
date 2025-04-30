package com.example.eeeeee;



import static android.content.ContentValues.TAG;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.bluetooth.*;
import android.Manifest;

import android.content.Intent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;


import androidx.activity.EdgeToEdge;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.core.graphics.Insets;
import androidx.core.view.GravityCompat;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.drawerlayout.widget.DrawerLayout;

import com.google.android.material.navigation.NavigationView;

import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    //Variables
    private static final int REQUEST_ENABLE_BT=10;

    private BluetoothAdapter bluetoothAdapter; //블루투스 어댑터
    private Set<BluetoothDevice> paired_devices;//블루투스 디바이스 셋
    private BluetoothDevice bluetoothDevice;//블루투스 디바이스

    private InputStream inputStream=null; //블루투스에 데이터를 입력하기 위한 입력 스트림
    private byte[] readBuffer;    // 수신된 문자열 저장 버퍼

    private int readBufferPosition; // 버퍼 내 문자 저장 위치
    String[] array={"0"};  // 수신된 문자열을 쪼개서 저장할 배열

    TextView temp_degree;
    TextView humi_per;
    TextView pl1_status;
    TextView pl2_status;
    TextView pl3_status;
    TextView pl4_status;
    TextView pl5_status;
    TextView vacant_status;
    TextView flame_status;
    TextView gate_status;

    private DrawerLayout drawer;


    private final ActivityResultLauncher<Intent> enableBluetoothLauncher =
            registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), result -> {
                if (result.getResultCode() == RESULT_OK) {
                    selectBluetoothDevice();
                } else {
                    Toast.makeText(this, "Bluetooth not enabled", Toast.LENGTH_SHORT).show();
                }
            });
    private final ActivityResultLauncher<String[]> requestPermissionsLauncher =
            registerForActivityResult(new ActivityResultContracts.RequestMultiplePermissions(), permissions -> {
                if (Boolean.TRUE.equals(permissions.getOrDefault(Manifest.permission.BLUETOOTH_CONNECT, false)) &&
                        Boolean.TRUE.equals(permissions.getOrDefault(Manifest.permission.ACCESS_FINE_LOCATION, false))) {
                    if (bluetoothAdapter.isEnabled()) {
                        selectBluetoothDevice();
                    } else {
                        Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        enableBluetoothLauncher.launch(enableIntent);
                    }
                } else {
                    Toast.makeText(this, "Required permissions denied", Toast.LENGTH_SHORT).show();
                }
            });






    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        // Toolbar 설정
        Toolbar toolbar = findViewById(R.id.toolbar);
        if (toolbar == null) {
            Log.e(TAG, "Toolbar is null. Check R.id.toolbar in activity_main.xml");
            Toast.makeText(this, "UI 초기화 실패: Toolbar", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        setSupportActionBar(toolbar);

        // DrawerLayout 설정
        drawer = findViewById(R.id.drawer_layout);
        if (drawer == null) {
            Log.e(TAG, "DrawerLayout is null. Check R.id.drawer_layout in activity_main.xml");
            Toast.makeText(this, "UI 초기화 실패: DrawerLayout", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        // ActionBarDrawerToggle 커스터마이징 (우측 드로어)
        ActionBarDrawerToggle toggle;
        try {
            toggle = new ActionBarDrawerToggle(
                    this, drawer, toolbar, R.string.open_drawer, R.string.close_drawer) {
                @Override
                public void onDrawerOpened(android.view.View drawerView) {
                    super.onDrawerOpened(drawerView);
                    Log.d(TAG, "Drawer opened (right)");
                }

                @Override
                public void onDrawerClosed(android.view.View drawerView) {
                    super.onDrawerClosed(drawerView);
                    Log.d(TAG, "Drawer closed (right)");
                }
            };
            drawer.addDrawerListener(toggle);
            toggle.syncState();

            // 햄버거 버튼 클릭 시 우측 드로어 열기
            toolbar.setNavigationOnClickListener(v -> {
                Log.d(TAG, "Hamburger button clicked, opening right drawer");
                drawer.openDrawer(GravityCompat.END);
            });
        } catch (Exception e) {
            Log.e(TAG, "Failed to initialize ActionBarDrawerToggle", e);
            Toast.makeText(this, "드로어 설정 실패", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        // NavigationView 메뉴 선택 리스너
        NavigationView navigationView = findViewById(R.id.nav_view);
        if (navigationView == null) {
            Log.e(TAG, "NavigationView is null. Check R.id.nav_view in activity_main.xml");
            Toast.makeText(this, "UI 초기화 실패: NavigationView", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        navigationView.setNavigationItemSelectedListener(item -> {
            try {
                int itemId = item.getItemId();
                Log.d(TAG, "Navigation item selected: " + itemId);
                if (itemId == R.id.nav_parking_status) {
                    // 이미 MainActivity, 드로어 닫기
                    drawer.closeDrawer(GravityCompat.END);
                } else if (itemId == R.id.nav_cctv) {
                    // CCTVActivity로 전환
                    startActivity(new Intent(MainActivity.this, CCTVActivity.class));
                    drawer.closeDrawer(GravityCompat.END);
                }
                return true;
            } catch (Exception e) {
                Log.e(TAG, "Navigation item selection failed", e);
                Toast.makeText(this, "메뉴 선택 실패", Toast.LENGTH_SHORT).show();
                return false;
            }
        });
        //UI 초기화 하기
        temp_degree=findViewById(R.id.textViewTemp);
        humi_per=findViewById(R.id.textViewHumi);
        pl1_status=findViewById(R.id.textViewPl1);
        pl2_status=findViewById(R.id.textViewPl2);
        pl3_status=findViewById(R.id.textViewPl3);
        pl4_status=findViewById(R.id.textViewPl4);
        pl5_status=findViewById(R.id.textViewPl5);
        vacant_status=findViewById(R.id.textViewEmpty);
        flame_status=findViewById(R.id.textViewFlame);
        gate_status=findViewById(R.id.textViewGate);



        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "This device doesn't support Bluetooth", Toast.LENGTH_SHORT).show();
            return;
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            checkAndRequestPermissions();
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.S)
    private void checkAndRequestPermissions() {
        List<String> permissionsNeeded = new ArrayList<>();
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            permissionsNeeded.add(Manifest.permission.BLUETOOTH_CONNECT);
        }
        if (ContextCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {
            permissionsNeeded.add(android.Manifest.permission.ACCESS_FINE_LOCATION);
        }
        if (!permissionsNeeded.isEmpty()) {
            requestPermissionsLauncher.launch(permissionsNeeded.toArray(new String[0]));
        } else {
            if (bluetoothAdapter.isEnabled()) {
                selectBluetoothDevice();
            } else {
                Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                enableBluetoothLauncher.launch(enableIntent);
            }
        }
    }

    @SuppressLint("MissingPermission")
    public void selectBluetoothDevice() {
        paired_devices = bluetoothAdapter.getBondedDevices();
        int pairedDeviceCount = paired_devices.size();

        if (pairedDeviceCount == 0) {
            Toast.makeText(this, "No paired devices found", Toast.LENGTH_SHORT).show();
            return;
        }

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("List of Paired Devices");
        List<String> list = new ArrayList<>();
        for (BluetoothDevice bluetoothDevice : paired_devices) {
            list.add(bluetoothDevice.getName());
        }
        list.add("Cancel");

        final CharSequence[] charSequences = list.toArray(new CharSequence[0]);
        builder.setItems(charSequences, (dialog, which) -> {
            if (which < list.size() - 1) {
                connectDevice(charSequences[which].toString());
            } else {
                dialog.dismiss();
            }
        });

        builder.setCancelable(false);
        AlertDialog alertDialog = builder.create();
        alertDialog.show();
    }

    @SuppressLint("MissingPermission")
    public void connectDevice(String deviceName) {
        for (BluetoothDevice tempDevice : paired_devices) {
            if (deviceName.equals(tempDevice.getName())) {
                bluetoothDevice = tempDevice;
                break;
            }
        }

        if (bluetoothDevice == null) {
            Toast.makeText(this, "Device not found", Toast.LENGTH_SHORT).show();
            return;
        }

        Toast.makeText(this, "Connecting to " + bluetoothDevice.getName(), Toast.LENGTH_SHORT).show();

        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

        try {
            //블루투스 소켓
            BluetoothSocket bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(uuid);
            bluetoothSocket.connect();
            //블루투스에 데이터를 출력하기 위한 출력 스트림
            OutputStream outputStream = bluetoothSocket.getOutputStream();
            inputStream = bluetoothSocket.getInputStream();
            receiveData();
        } catch (IOException e) {
            Log.e("BluetoothError", "Failed to connect: " + e.getMessage(), e);
            Toast.makeText(this, "Connection failed", Toast.LENGTH_SHORT).show();
        }
    }



    public void receiveData() {
        final Handler handler = new Handler(Looper.getMainLooper());
        // 데이터 수신을 위한 버퍼 생성
        readBufferPosition = 0;
        readBuffer = new byte[1024];

        // 데이터 수신을 위한 쓰레드 생성
        // 데이터 수신 확인
        // 데이터 수신된 경우
        // 입력 스트림에서 바이트 단위로 읽어옴
        // 반환값 사용
        // 스트림 종료 처리
        // 입력 스트림 바이트를 한 바이트씩 읽어옴
        // 개행문자를 기준으로 받음 (한줄)
        // readBuffer 배열을 encodedBytes로 복사
        // 인코딩된 바이트 배열을 문자열로 변환
        // UI 업데이트 (예: TextView에 텍스트 설정)
        // 예: textView.setText(text);
        // 1초 대기
        // IOException 발생 시 루프 종료
        // 쓰레드 중단
        //문자열 수신에 사용되는 쓰레드
        Thread workerThread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    // 데이터 수신 확인
                    int byteAvailable = inputStream.available();
                    // 데이터 수신된 경우
                    if (byteAvailable > 0) {
                        // 입력 스트림에서 바이트 단위로 읽어옴
                        byte[] bytes = new byte[byteAvailable];
                        int bytesRead = inputStream.read(bytes); // 반환값 사용
                        if (bytesRead == -1) {
                            // 스트림 종료 처리
                            handler.post(() -> Toast.makeText(this, "Input stream closed", Toast.LENGTH_SHORT).show());
                            break;
                        }
                        // 입력 스트림 바이트를 한 바이트씩 읽어옴
                        for (int i = 0; i < bytesRead; i++) {
                            byte tempByte = bytes[i];
                            // 개행문자를 기준으로 받음 (한줄)
                            if (tempByte == '\n') {
                                // readBuffer 배열을 encodedBytes로 복사
                                byte[] encodedBytes = new byte[readBufferPosition];
                                System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                // 인코딩된 바이트 배열을 문자열로 변환
                                final String text = new String(encodedBytes, StandardCharsets.UTF_8);

                                readBufferPosition = 0;
                                handler.post(() -> {
                                    // UI 업데이트 (예: TextView에 텍스트 설정)
                                    //text에 데이터 포맷 맞춰서 수신

                                    try {
                                        Map<String, String> parsedData = SensorDataParser.parse(text);
                                        handler.post(() -> updateUI(parsedData));
                                        Log.d(TAG, "Received and parsed: " + parsedData);
                                    } catch (IllegalArgumentException e) {
                                        Log.e(TAG, "Parse error: " + e.getMessage());
                                        handler.post(() -> Toast.makeText(this, "Invalid data: " + e.getMessage(), Toast.LENGTH_SHORT).show());
                                    }
                                    // 예: textView.setText(text);
                                    Log.d("BluetoothData", "Received: " + text);
                                });
                            } else {
                                readBuffer[readBufferPosition++] = tempByte;
                            }
                        }
                    }
                    // 1초 대기
                    Thread.sleep(1000);
                } catch (IOException e) {
                    Log.e("BluetoothError", "Failed to read data: " + e.getMessage(), e);
                    handler.post(() -> Toast.makeText(this, "Failed to read data", Toast.LENGTH_SHORT).show());
                    break; // IOException 발생 시 루프 종료
                } catch (InterruptedException e) {
                    Log.e("BluetoothError", "Thread interrupted: " + e.getMessage(), e);
                    handler.post(() -> Toast.makeText(this, "Data receive interrupted", Toast.LENGTH_SHORT).show());
                    break; // 쓰레드 중단
                }
            }
        });
        workerThread.start();
    }


    private void updateUI(Map<String, String> parsedData){
        temp_degree.setText(getString(R.string.temp_format, parsedData.get("Temp")));
        humi_per.setText(getString(R.string.humi_format, parsedData.get("Humi") != null ? parsedData.get("Humi") : "--"));
        pl1_status.setText(parsedData.get("pl1") != null ? parsedData.get("pl1") : "--");
        pl2_status.setText(parsedData.get("pl2") != null ? parsedData.get("pl2") : "--");
        pl3_status.setText(parsedData.get("pl3") != null ? parsedData.get("pl3") : "--");
        pl4_status.setText(parsedData.get("pl4") != null ? parsedData.get("pl4") : "--");
        pl5_status.setText(parsedData.get("pl5") != null ? parsedData.get("pl5") : "--");

        vacant_status.setText(getString(R.string.empty_format, parsedData.get("empty") != null ? parsedData.get("empty") : "--"));
        flame_status.setText(parsedData.get("flame") != null ? parsedData.get("flame") : "--");
        gate_status.setText(getString(R.string.gate_format, parsedData.get("Main") != null ? parsedData.get("Main") : "--"));


        if ("화재발생".equals(parsedData.get("flame"))) {
            flame_status.setBackgroundColor(getResources().getColor(android.R.color.holo_red_light));
            Toast.makeText(this, "화재 발생 감지!", Toast.LENGTH_SHORT).show();
        } else {
            flame_status.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
        }
    }



}