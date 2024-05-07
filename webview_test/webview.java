package com.example.webview;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;

public class WebViewAct extends AppCompatActivity {

    WebView webview1;
    Button btnb, btnf, btnh, btnn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_web_view);

        Intent intent = getIntent();
        String url = intent.getStringExtra("URL");

        webview1 = findViewById(R.id.web1);
        btnb = findViewById(R.id.btnback);
        btnf = findViewById(R.id.btnfor);
        btnh = findViewById(R.id.btnhome);
        btnn = findViewById(R.id.btnnew);

        WebSettings webSettings = webview1.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webview1.loadUrl(url);
        webview1.setWebViewClient(new WebViewClient());
        webview1.setWebViewClient(new MyAppWebViewClient(url));

        btnb.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(webview1.canGoBack()){
                    webview1.goBack();
                }else{

                }
            }
        });

        btnf.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(webview1.canGoForward()){
                    webview1.goForward();
                }else{

                }
            }
        });

        btnh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                webview1.loadUrl(url);
               // webview1.setWebViewClient(new MyAppWebViewClient(url));
            }
        });

        btnn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });



    }
    @Override
    public void onBackPressed(){
        if(webview1.canGoBack()){
            webview1.goBack();
        }else{
            super.onBackPressed();
        }
    }
}
