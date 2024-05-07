package com.example.post_requesttest;

import com.google.gson.annotations.SerializedName;

public class AdafruitData {
   @SerializedName("value")
    private String value;
   public AdafruitData(String value){
       this.value = value;
   }
}
