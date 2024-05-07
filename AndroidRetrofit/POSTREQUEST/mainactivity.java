package com.example.post_requesttest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {

    private Button btn1;
    private static final String baseurl = "https://io.adafruit.com/api/v2/";
    //https://io.adafruit.com/api/v2/
    private static final String AIO_Key = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn1 = findViewById(R.id.button);
        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sendData("1");
            }
        });

    }

    private void sendData(String value){
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(baseurl)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        post_interface api_service = retrofit.create(post_interface.class);
        AdafruitData data = new AdafruitData(value);
        Call<Void> call = api_service.sendData(AIO_Key, data);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> call, Response<Void> response) {
                int statusCode = response.code();
                if(response.isSuccessful()){
                    Log.d("POST", "SUCCESS "+statusCode);
                }else{
                    Log.d("POST", "FAILURE "+statusCode);
                }
            }

            @Override
            public void onFailure(Call<Void> call, Throwable t) {
                Log.d("POST", "FAILURE Network"+t);
            }
        });
    }
}
