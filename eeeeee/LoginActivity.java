package com.example.eeeeee;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

import com.example.eeeeee.R;

import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
import okhttp3.logging.HttpLoggingInterceptor;
import org.json.JSONObject;
import java.io.IOException;
import java.util.concurrent.TimeUnit;

public class LoginActivity extends AppCompatActivity {
    private EditText editTextUsername, editTextPassword;
    private Button buttonLogin;
    private OkHttpClient client;
    private static final MediaType JSON = MediaType.parse("application/json; charset=utf-8");
    private static final String SERVER_URL = "http://10.10.10.106:5001";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("LOGIN_ACTIVITY", "onCreate called");
        setContentView(R.layout.activity_login);

        // UI 요소 초기화
        try {
            editTextUsername = findViewById(R.id.editTextUsername);
            editTextPassword = findViewById(R.id.editTextPassword);
            buttonLogin = findViewById(R.id.buttonLogin);
            Log.d("LOGIN_ACTIVITY", "UI elements initialized: buttonLogin ID=" + R.id.buttonLogin);
        } catch (Exception e) {
            Log.e("LOGIN_ERROR", "Error initializing UI: " + e.getMessage(), e);
            Toast.makeText(this, "UI 초기화 오류: " + e.getMessage(), Toast.LENGTH_LONG).show();
            return;
        }

        // OkHttp 클라이언트 초기화 (로깅 인터셉터 포함)
        HttpLoggingInterceptor logging = new HttpLoggingInterceptor(message -> Log.d("OKHTTP", message));
        logging.setLevel(HttpLoggingInterceptor.Level.BODY);
        client = new OkHttpClient.Builder()
                .addInterceptor(logging)
                .connectTimeout(30, TimeUnit.SECONDS)
                .readTimeout(30, TimeUnit.SECONDS)
                .writeTimeout(30, TimeUnit.SECONDS)
                .build();

        // 로그인 버튼 클릭 리스너
        buttonLogin.setOnClickListener(v -> {
            Log.d("LOGIN_ACTIVITY", "Login button clicked");
            String id = editTextUsername.getText().toString().trim();
            String password = editTextPassword.getText().toString().trim();

            if (id.isEmpty() || password.isEmpty()) {
                Log.w("LOGIN_ACTIVITY", "Empty id or password");
                Toast.makeText(LoginActivity.this, "Please enter username and password", Toast.LENGTH_SHORT).show();
                return;
            }

            // JSON 요청 본문 생성
            JSONObject jsonBody = new JSONObject();
            try {
                jsonBody.put("id", id);
                jsonBody.put("password", password);
                Log.d("LOGIN_REQUEST", "Request Body: " + jsonBody.toString());
            } catch (Exception e) {
                Log.e("LOGIN_ERROR", "Error creating JSON: " + e.getMessage(), e);
                Toast.makeText(LoginActivity.this, "JSON 생성 오류: " + e.getMessage(), Toast.LENGTH_SHORT).show();
                return;
            }

            // 비동기 요청 실행
            new LoginTask().execute(jsonBody.toString());
        });
    }

    @SuppressLint("StaticFieldLeak")
    private class LoginTask extends AsyncTask<String, Void, String> {
        private String errorMessage = null;

        @Override
        protected String doInBackground(String... params) {
            String jsonBody = params[0];
            String url = SERVER_URL + "/login";
            Log.d("LOGIN_REQUEST", "Request URL: " + url);

            RequestBody body = RequestBody.create(JSON, jsonBody); // Fixed order of parameters
            Request request = new Request.Builder()
                    .url(url)
                    .post(body)
                    .addHeader("Content-Type", "application/json; charset=UTF-8")
                    .build();

            try {
                Response response = client.newCall(request).execute();
                Log.d("LOGIN_RESPONSE", "Response Code: " + response.code());
                String responseBody = response.body() != null ? response.body().string() : "";
                Log.d("LOGIN_RESPONSE", "Response Body: " + responseBody);

                if (!response.isSuccessful()) {
                    errorMessage = "Server error: " + response.code() + " - " + responseBody;
                    return null;
                }

                return responseBody;
            } catch (IOException e) {
                Log.e("LOGIN_ERROR", "Network error: " + e.getMessage(), e);
                errorMessage = "네트워크 오류: " + e.getMessage();
                return null;
            }
        }

        @Override
        protected void onPostExecute(String result) {
            if (result == null) {
                Toast.makeText(LoginActivity.this, errorMessage != null ? errorMessage : "알 수 없는 오류", Toast.LENGTH_LONG).show();
                return;
            }

            try {
                JSONObject response = new JSONObject(result);
                if (response.has("message")) {
                    String name = response.getString("name");
                    String userId = response.getString("id");
                    Toast.makeText(LoginActivity.this, "반갑습니다!! " + name, Toast.LENGTH_LONG).show();

                    // MainActivity로 이동
                    Intent intent = new Intent(LoginActivity.this, MainActivity.class);
                    intent.putExtra("USER_NAME", name);
                    intent.putExtra("USER_ID", userId);
                    startActivity(intent);
                    finish();
                } else if (response.has("error")) {
                    Log.w("LOGIN_RESPONSE", "Error: " + response.getString("error"));
                    Toast.makeText(LoginActivity.this, response.getString("error"), Toast.LENGTH_LONG).show();
                }
            } catch (Exception e) {
                Log.e("LOGIN_ERROR", "Error parsing response: " + e.getMessage(), e);
                Toast.makeText(LoginActivity.this, "응답 처리 오류: " + e.getMessage(), Toast.LENGTH_SHORT).show();
            }
        }
    }
}