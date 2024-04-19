package com.example.retrofitget;
import retrofit2.Call;
import retrofit2.http.GET;

public interface Methods {
    //@GET("/state1")
    @GET("state1")
    Call<Model> getLastValue();
}
