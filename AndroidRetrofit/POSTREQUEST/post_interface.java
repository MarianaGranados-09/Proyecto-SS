package com.example.post_requesttest;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.Header;
import retrofit2.http.Headers;
import retrofit2.http.POST;

public interface post_interface {
    @POST("MarianaGrad09/feeds/pruebapost/data")
    Call<Void> sendData(
            @Header("X-AIO-Key") String apiKey,
            @Body AdafruitData data
    );
}
