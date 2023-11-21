#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
    
    uint8_t pin_alarm = 5;
    uint8_t pin_motion = 4;
    uint8_t pin_pres = 10;
    char* ssid = "electro";
    char* password = "12345678";
    String alarm_state;
    String motion_state;
    String pres_state;
    String total_state;
    
    char webpage[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>PETFED</title>
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
      pinMode(pin_pres, INPUT);
      WiFi.begin(ssid,password);

      Serial.begin(115200);
      while(WiFi.status()!=WL_CONNECTED)
      { 
        Serial.print(".");
        delay(500);
      }
      Serial.println("");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    
      server.on("/",[](){server.send_P(200,"text/html", webpage);});
      server.on("/state",getState);
      server.begin();
    }
    
    void loop()
    {
      server.handleClient();
      getState();
    }
    
    
    void getState()
    {
      /*
      alarm_state = digitalRead(pin_alarm) ? "{\n \t\"gas\": 1 \n" : "{\n \t\"gas\": 0, \n";
      motion_state = digitalRead(pin_motion) ? "\n \t\"motion\": 1 \n" : "\n \t\"motion\": 0, \n";
      pres_state = digitalRead(pin_pres) ? "\n \t\"presence\": 1 \n}" : "\n \t\"presence\": 0 \n}";
      */
      alarm_state = "{\n \t gas: 1, \n";
      motion_state = "\n \t motion: 0, \n";
      pres_state = "\n \t presence: 1 \n}";

      total_state = alarm_state+motion_state+pres_state;

      server.send(200,"text/plain", total_state);
    }
