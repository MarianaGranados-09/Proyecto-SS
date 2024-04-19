package com.example.retrofitget;

import com.google.gson.annotations.SerializedName;

public class Model {
        @SerializedName("last_value")
        private String last_value;

        // Getter and setter for name field
        public String getLast_value() {
            return last_value;
        }

        public void setLast_value(String last_value) {
            this.last_value = last_value;
        }
}
