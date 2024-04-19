package com.example.retrofitget;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {

    private TextView nameTextView;
    private TextView emailTextView;
    private Button fetchDataButton;

    private Methods apiService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nameTextView = findViewById(R.id.txt1);
        fetchDataButton = findViewById(R.id.btnGetData);

        // Create Retrofit instance
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("https://io.adafruit.com/api/v2/MarianaGrad09/feeds/")
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        // Create an instance of the ApiService interface
        apiService = retrofit.create(Methods.class);

        // Set click listener for the button
        fetchDataButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                fetchData();
            }
        });
    }

    private void fetchData() {
        // Make API call
        Call<Model> call = apiService.getLastValue();

        call.enqueue(new Callback<Model>() {
            @Override
            public void onResponse(Call<Model> call, Response<Model> response) {
                if (response.isSuccessful()) {
                    Model user = response.body();
                    // Update UI with user data
                    nameTextView.setText("State: "+user.getLast_value());
                } else {
                    Toast.makeText(MainActivity.this, "Failed to fetch data", Toast.LENGTH_SHORT).show();
                }
            }

            @Override
            public void onFailure(Call<Model> call, Throwable t) {
                Toast.makeText(MainActivity.this, "Error occurred", Toast.LENGTH_SHORT).show();
            }
        });
    }
}

