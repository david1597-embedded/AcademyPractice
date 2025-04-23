package com.example.parkingapp;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BluetoothManager {
    private static final String TAG = "BluetoothManager";
    private static BluetoothManager instance;

    private BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;

    // 싱글톤 인스턴스 접근 메소드
    public static synchronized BluetoothManager getInstance() {
        if (instance == null) {
            instance = new BluetoothManager();
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
        } catch (IOException e) {
            Log.e(TAG, "블루투스 스트림 가져오기 실패", e);
        }
    }

    // 블루투스 소켓 가져오기
    public BluetoothSocket getBluetoothSocket() {
        return bluetoothSocket;
    }

    // 입력 스트림 가져오기
    public InputStream getInputStream() {
        return inputStream;
    }

    // 출력 스트림 가져오기
    public OutputStream getOutputStream() {
        return outputStream;
    }

    // 메시지 보내기
    public boolean sendMessage(String message) {
        if (bluetoothSocket == null || !bluetoothSocket.isConnected() || outputStream == null) {
            Log.e(TAG, "블루투스가 연결되지 않았습니다.");
            return false;
        }

        try {
            outputStream.write(message.getBytes());
            return true;
        } catch (IOException e) {
            Log.e(TAG, "메시지 전송 실패", e);
            return false;
        }
    }

    // 연결 종료
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
        } catch (IOException e) {
            Log.e(TAG, "블루투스 연결 종료 실패", e);
        }

        inputStream = null;
        outputStream = null;
        bluetoothSocket = null;
    }

    // 연결 상태 확인
    public boolean isConnected() {
        return bluetoothSocket != null && bluetoothSocket.isConnected();
    }
}
