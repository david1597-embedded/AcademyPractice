package com.example.parkingapp;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.io.IOException;
import java.io.InputStream;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicBoolean;

/** @noinspection ALL*/
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private TextView tvTemp, tvHumi, tvPl1Status, tvPl2Status, tvPl3Status, tvPl4Status, tvPl5Status, tvPlSpace, tvGateStatus, tvFlameStatus;
    private InputStream inputStream;
    private Thread workerThread;
    private byte[] readBuffer;
    private int readBufferPosition;
    private AtomicBoolean stopWorker;
    private Handler handler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        handler = new Handler(Looper.getMainLooper());

        // 툴바 설정
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        Objects.requireNonNull(getSupportActionBar()).setDisplayShowTitleEnabled(false);

        // Window Insets 처리
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        initViews();

        if (BluetoothManager.getInstance().isConnected()) {
            Toast.makeText(this, "No Bluetooth Connection. Connect again.", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        beginListenForData();
    }

    private void initViews() {
        tvTemp = findViewById(R.id.tv_temp);
        tvHumi = findViewById(R.id.tv_humi);
        tvPl1Status = findViewById(R.id.tv_pl1_status);
        tvPl2Status = findViewById(R.id.tv_pl2_status);
        tvPl3Status = findViewById(R.id.tv_pl3_status);
        tvPl4Status = findViewById(R.id.tv_pl4_status);
        tvPl5Status = findViewById(R.id.tv_pl5_status);
        tvPlSpace = findViewById(R.id.tv_pl_space);
        tvGateStatus = findViewById(R.id.tv_gate_status);
        tvFlameStatus = findViewById(R.id.tv_flame_status);
    }

    private void beginListenForData() {
        stopWorker = new AtomicBoolean(false);
        readBufferPosition = 0;
        readBuffer = new byte[1024];

        inputStream = BluetoothManager.getInstance().getInputStream();
        if (inputStream == null) {
            Toast.makeText(this, "Cannot access Bluetooth input stream", Toast.LENGTH_LONG).show();
            return;
        }

        workerThread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted() && !stopWorker.get()) {
                try {
                    int bytesAvailable = inputStream.available();
                    if (bytesAvailable > 0) {
                        byte[] packetBytes = new byte[bytesAvailable];
                        int bytesRead = inputStream.read(packetBytes);
                        if (bytesRead == -1) {
                            throw new IOException("Input stream closed");
                        }

                        for (int i = 0; i < bytesRead; i++) {
                            byte b = packetBytes[i];
                            if (b == '\n') {
                                byte[] encodedBytes = new byte[readBufferPosition];
                                System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                final String data = new String(encodedBytes, "UTF-8");
                                readBufferPosition = 0;
                                handler.post(() -> processReceivedData(data));
                            } else {
                                readBuffer[readBufferPosition++] = b;
                            }
                        }
                    }
                    Thread.sleep(100);
                } catch (IOException e) {
                    stopWorker.set(true);
                    Log.e(TAG, "Bluetooth read error", e);
                    handler.post(() -> {
                        Toast.makeText(MainActivity.this, "Bluetooth disconnected", Toast.LENGTH_LONG).show();
                        BluetoothManager.getInstance().closeConnection();
                    });
                } catch (InterruptedException e) {
                    stopWorker.set(true);
                    Log.d(TAG, "Data read thread interrupted");
                }
            }
        });
        workerThread.start();
    }

    private void processReceivedData(String data) {
        Log.d(TAG, "Received data: " + data);
        try {
            String[] parts = data.split("/");
            if (parts.length < 10) {
                Log.e(TAG, "Not supported data format: " + data);
                return;
            }

            String tempPart = parts[0].trim();
            int tempValue = Integer.parseInt(tempPart.substring(tempPart.indexOf(":") + 1));
            tvTemp.setText("주차장 내부 온도: " + tempValue + "°C");

            String humiPart = parts[1].trim();
            int humiValue = Integer.parseInt(humiPart.substring(humiPart.indexOf(":") + 1));
            tvHumi.setText("주차장 내부 습도: " + humiValue + "%");

            updateParkingStatus(1, parts[2]);
            updateParkingStatus(2, parts[3]);
            updateParkingStatus(3, parts[4]);
            updateParkingStatus(4, parts[5]);
            updateParkingStatus(5, parts[6]);

            String emptyPart = parts[7].trim();
            int emptyValue = Integer.parseInt(emptyPart.substring(emptyPart.indexOf(":") + 1));
            tvPlSpace.setText("현재 주차 가능 공간: " + emptyValue);

            String flamePart = parts[8].trim();
            int flameValue = Integer.parseInt(flamePart.substring(flamePart.indexOf(":") + 1));
            if (flameValue == 1) {
                tvFlameStatus.setText("화재 경보 유무: 화재 경보 발생!");
                tvFlameStatus.setTextColor(getResources().getColor(android.R.color.holo_red_dark));
            } else {
                tvFlameStatus.setText("화재 경보 유무: 정상");
                tvFlameStatus.setTextColor(getResources().getColor(android.R.color.black));
            }

            String mainPart = parts[9].trim();
            String mainStatus = mainPart.substring(mainPart.indexOf(":") + 1);
            tvGateStatus.setText("정문 게이트 상태: " + mainStatus);
        } catch (Exception e) {
            Log.e(TAG, "Error data processing", e);
            Toast.makeText(this, "Error data processing: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    private void updateParkingStatus(int slotNumber, String statusData) {
        String statusPart = statusData.trim();
        int statusValue = Integer.parseInt(statusPart.substring(statusPart.indexOf(":") + 1));
        TextView targetTextView = null;

        switch (slotNumber) {
            case 1:
                targetTextView = tvPl1Status;
                break;
            case 2:
                targetTextView = tvPl2Status;
                break;
            case 3:
                targetTextView = tvPl3Status;
                break;
            case 4:
                targetTextView = tvPl4Status;
                break;
            case 5:
                targetTextView = tvPl5Status;
                break;
        }

        if (targetTextView != null) {
            if (statusValue == 1) {
                targetTextView.setText("점유");
                targetTextView.setBackgroundResource(R.drawable.occupied_border);
            } else {
                targetTextView.setText("공차");
                targetTextView.setBackgroundResource(R.drawable.empty_border);
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.menu_parking_status) {
            return true;
        } else if (id == R.id.menu_cctv) {
            Intent intent = new Intent(MainActivity.this, CCTVListActivity.class);
            startActivity(intent);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stopWorker.set(true);
        if (workerThread != null) {
            workerThread.interrupt();
        }
        BluetoothManager.getInstance().closeConnection();
    }
}