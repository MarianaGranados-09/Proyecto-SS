#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

    IPAddress local_IP(192, 168, 137, 242);
    // Set your Gateway IP address
    IPAddress gateway(192, 168, 1, 1);

    IPAddress subnet(255, 255, 0, 0);
    IPAddress primaryDNS(8, 8, 8, 8);   //optional
    IPAddress secondaryDNS(8, 8, 4, 4); //optional

    
    uint8_t pin_alarm = 14;
    uint8_t pin_motion = 10;
    uint8_t pin_pres = 4;
    uint8_t pin_btn = 5;
    uint8_t pin_LED = 12;
    
    int flag = 0;


    char* ssid = "redmi_ap";
    char* password = "PCUAQ2024";
    String alarm_state;
    String motion_state;
    String pres_state;
    String total_state;
    String btn_state;
    
    char webpage[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Sensor states</title>
    </head>
    <body>
    </body>
    
    </html>
    <script>
    function myFunction()
    {
      console.log("boton presionado");
      var xhr = new XMLHttpRequest();
      var url = "/ledstate";
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("led-state").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", url, true);
      xhr.send();
    };
    document.addEventListener('DOMContentLoaded', myFunction, false);
    </script>
    </html>
    )=====";
    
    void setup()
    {
      pinMode(pin_alarm, INPUT);
      pinMode(pin_motion, INPUT);
      pinMode(pin_pres, INPUT_PULLUP);
      pinMode(pin_btn, INPUT_PULLUP);
      pinMode(pin_LED, OUTPUT);

      WiFi.begin(ssid,password);
      

      Serial.begin(115200);
      while(WiFi.status()!=WL_CONNECTED)
      { 
        digitalWrite(pin_LED, LOW);
        Serial.print(".");
        delay(500);
      }
      Serial.println("");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      digitalWrite(pin_LED, HIGH);
      
    
      server.on("/",[](){server.send_P(200,"text/html", webpage);});
      server.on("/state",getState);
      server.begin();
    }
    
    void loop()
    {
      if(WiFi.status()!=WL_CONNECTED)
      {
          digitalWrite(pin_LED, LOW);
          //Serial.print(".");
      }
      else
      {
        digitalWrite(pin_LED, HIGH);
        //Serial.println("");
        //Serial.print("IP Address: ");
        //Serial.println(WiFi.localIP());
        server.handleClient();
        getState();
      }
    }
    
    
    void getState()
    {

      int pinState_A = digitalRead(pin_alarm);
      int pinState_P = digitalRead(pin_pres);
      int pinState_B = digitalRead(pin_btn);
      
      
      /*
      alarm_state = digitalRead(pin_alarm) ? "{\n \t\"gas\": 1 \n" : "{\n \t\"gas\": 0, \n";
      motion_state = digitalRead(pin_motion) ? "\n \t\"motion\": 1 \n" : "\n \t\"motion\": 0, \n";
      pres_state = digitalRead(pin_pres) ? "\n \t\"presence\": 1 \n}" : "\n \t\"presence\": 0 \n}";
      */
     // alarm_state = "{\n \t gas: 1, \n";
     // motion_state = digitalRead(pin_motion) ? "\n \t\"motion\": 1 \n" : "\n \t\"motion\": 0, \n";
     if (pinState_A == HIGH) {
        alarm_state = "{\n \t alarm: 1, \n";
        ///////Serial.println("High level detected on smoke alarm");
      } else {
        alarm_state = "{\n \t alarm: 0, \n";
        //Serial.println("LOW level detected on smoke alarm");
      }
      if (pinState_P == HIGH) {
        pres_state = "\n\t window: 1, \n";
        //Serial.println("High level detected on window sensor");
      } else {
        pres_state = "\n\t window: 0,\n";
        //Serial.println("LOW level detected on window sensor");
      }
      if (pinState_B == HIGH) {
        flag = 1;
        //btn_state = "\n\t emergency: 1 \n}";
        //Serial.println("High level detected on emergency btn");
      }
      if (flag == 1) {
        //flag = 1;
        btn_state = "\n\t emergency: 1 \n}";
        //Serial.println("High level detected on emergency btn");
      } else {
       // flag = 0;
        btn_state = "\n\t emergency: 0 \n}";
        //Serial.println("LOW level detected on emergency btn");
      }

      motion_state = "\n\t pir: 0, \n";
      //pres_state = "\n \t presence: 1 \n}";-

      total_state = alarm_state+motion_state+pres_state+btn_state;

      server.send(200,"text/plain", total_state);
    }
