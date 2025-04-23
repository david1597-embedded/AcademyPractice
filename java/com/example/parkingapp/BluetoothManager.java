package com.example.parkingapp;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BluetoothManager {
    private static final String TAG = "BluetoothManager";
    private static volatile BluetoothManager instance;

    private BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;

    // 싱글톤 인스턴스 접근 메소드
    public static BluetoothManager getInstance() {
        if (instance == null) {
            synchronized (BluetoothManager.class) {
                if (instance == null) {
                    instance = new BluetoothManager();
                    Log.d(TAG, "New instance created");
                }
            }
        }
        return instance;
    }

    private BluetoothManager() {
        // private 생성자
    }

    // 블루투스 소켓 설정
    public void setBluetoothSocket(BluetoothSocket socket) {
        this.bluetoothSocket = socket;
        try {
            this.inputStream = socket.getInputStream();
            this.outputStream = socket.getOutputStream();
            Log.d(TAG, "Bluetooth streams initialized");
        } catch (IOException e) {
            Log.e(TAG, "Failed to initialize streams", e);
            this.inputStream = null;
            this.outputStream = null;
        }
    }



    // Get bluetooth socket
    public BluetoothSocket getBluetoothSocket() {
        return bluetoothSocket;
    }

    // Get Input Stream
    public InputStream getInputStream() {
        return inputStream;
    }

    // Get Output Stream
    public OutputStream getOutputStream() {
        return outputStream;
    }

    // 메시지 보내기
    public boolean sendMessage(String message) {
        if (bluetoothSocket == null || !isConnected() || outputStream == null) {
            Log.e(TAG, "Bluetooth not connected");
            return false;
        }
        try {
            outputStream.write(message.getBytes());
            Log.d(TAG, "Message sent: " + message);
            return true;
        } catch (IOException e) {
            Log.e(TAG, "Failed to send message", e);
            return false;
        }
    }


    // Terminate Connection
    public void closeConnection() {
        try {
            if (inputStream != null) {
                inputStream.close();
            }
            if (outputStream != null) {
                outputStream.close();
            }
            if (bluetoothSocket != null) {
                bluetoothSocket.close();
            }
            Log.d(TAG, "Bluetooth connection closed");
        } catch (IOException e) {
            Log.e(TAG, "Failed to close connection", e);
        }
        inputStream = null;
        outputStream = null;
        bluetoothSocket = null;
    }

    // Check Connection Status
    public boolean isConnected() {
        boolean connected = bluetoothSocket != null && bluetoothSocket.isConnected();
        Log.d(TAG, "isConnected: " + connected);
        return connected;
    }
}
