package com.example.parkingapp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class LoadingActivity extends AppCompatActivity {
    private static final String TAG = "LoadingActivity";
    private static final String BLUETOOTH_DEVICE_NAME = "MOON";
    private static final String STREAMING_SERVER_URL = "http://10.10.10.106:5000";
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private boolean isBluetoothConnected = false;
    private boolean isStreamingServerConnected = false;

    private BluetoothSocket bluetoothSocket;
    private BluetoothDevice bluetoothDevice;

    private TextView tvConnectionStatus;
    private ExecutorService executorService;
    private Handler handler;

    @RequiresApi(api = Build.VERSION_CODES.S)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loading);

        tvConnectionStatus = findViewById(R.id.tv_connection_status);
        handler = new Handler(Looper.getMainLooper());
        executorService = Executors.newFixedThreadPool(2);

        // 블루투스 권한 확인
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED ||
                ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{
                    Manifest.permission.BLUETOOTH_CONNECT,
                    Manifest.permission.BLUETOOTH_SCAN
            }, 1);
        } else {
            testConnections();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 1 && grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            testConnections();
        } else {
            Toast.makeText(this, "Need Bluetooth Authentication", Toast.LENGTH_LONG).show();
            finish();
        }
    }

    private void testConnections() {
        updateStatus("Bluetooth and Streaming server connection test...");

        executorService.execute(this::testBluetoothConnection);
        executorService.execute(this::testStreamingServerConnection);
        handler.postDelayed(this::checkConnectionStatus, 5000);
    }

    private void testBluetoothConnection() {
        try {
            updateStatus("Checking Bluetooth adapter...");

            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            if (bluetoothAdapter == null) {
                updateStatus("This Device does not support bluetooth...");
                return;
            }
            if (!bluetoothAdapter.isEnabled()) {
                updateStatus("Bluetooth turned off. Please turn bluetooth on!");
                return;
            }

            updateStatus("Searching Paired Device...");

            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                return;
            }
            Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();

            if (!pairedDevices.isEmpty()) {
                for (BluetoothDevice device : pairedDevices) {
                    if (device.getName() != null && device.getName().equals(BLUETOOTH_DEVICE_NAME)) {
                        bluetoothDevice = device;
                        updateStatus("Found Bluetooth Device. Trying to connect...");
                        break;
                    }
                }

                if (bluetoothDevice == null) {
                    updateStatus("Cannot find Bluetooth device. Check Pairing");
                    return;
                }

                bluetoothSocket = bluetoothDevice.createInsecureRfcommSocketToServiceRecord(MY_UUID);
                bluetoothAdapter.cancelDiscovery();
                bluetoothSocket.connect();

                if (bluetoothSocket.isConnected()) {
                    updateStatus("Bluetooth Connection Success!");
                    isBluetoothConnected = true;
                    BluetoothManager.getInstance().setBluetoothSocket(bluetoothSocket);
                }
            } else {
                updateStatus("No Paired bluetooth Device. Pair device from Bluetooth Setting.");
            }
        } catch (IOException e) {
            Log.e(TAG, "Failed to connect Bluetooth", e);
            updateStatus("Failed to connect bluetooth: " + e.getMessage());
            try {
                if (bluetoothSocket != null) {
                    bluetoothSocket.close();
                }
            } catch (IOException closeException) {
                Log.e(TAG, "Failed to close bluetooth socket", closeException);
            }
        } catch (Exception e) {
            Log.e(TAG, "Error occurred during bluetooth test", e);
            updateStatus("Error occurred during bluetooth test: " + e.getMessage());
        }
    }

    private void testStreamingServerConnection() {
        try {
            updateStatus("Testing Connection to Streaming service...");

            URL url = new URL(STREAMING_SERVER_URL);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.setConnectTimeout(5000);
            connection.setReadTimeout(5000);

            int responseCode = connection.getResponseCode();
            connection.disconnect();

            if (responseCode == HttpURLConnection.HTTP_OK) {
                updateStatus("Connection to Live server Success!");
                isStreamingServerConnected = true;
            } else {
                updateStatus("Connection to Live server Failed. Response Code: " + responseCode);
            }
        }
         catch (Exception e) {
            Log.e(TAG, "Error occurred during Live Server test", e);
            updateStatus("Error occurred during Live Server test: " + e.getMessage());
        }
    }

    private void updateStatus(final String message) {
        handler.post(() -> {
            Log.d(TAG, message);
            tvConnectionStatus.setText(message);
        });
    }

    private void checkConnectionStatus() {
        if (isBluetoothConnected && isStreamingServerConnected) {
            updateStatus("All connections Success. Moving to Main...");

            handler.postDelayed(() -> {
                Intent intent = new Intent(LoadingActivity.this, MainActivity.class);
                startActivity(intent);
                finish();
            }, 1000);
        } else {
            String failMessage = "Connection failed: ";
            if (!isBluetoothConnected) {
                failMessage += "Bluetooth Connection failed.";
            }
            if (!isStreamingServerConnected) {
                failMessage += "Streaming Server Connection failed.";
            }
            updateStatus(failMessage + "\nRetrying Connection...");

            handler.postDelayed(this::testConnections, 5000);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        executorService.shutdown();
        if (!isBluetoothConnected && bluetoothSocket != null) {
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "Failed to close bluetooth socket", e);
            }
        }
    }
}