#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

IPAddress local_IP(192, 168, 137, 242);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


#include "config.h"


uint8_t pinAH = 14; //pin alarma de humo
uint8_t pinBP = 13; //pin del boton de panico
uint8_t pinSM = 2; //pin del sensor de movimiento
uint8_t pinSMG = 4; //pin del sensor magnetico
uint8_t pinLED = 12; //pin del led indicador


int flagbtn = 0;

// feed para alarma de humo
AdafruitIO_Feed *humo = io.feed("alarmaHumo");
//feed para boton de panico
AdafruitIO_Feed *boton = io.feed("btnPanico");
//feed para sensor de movimiento
AdafruitIO_Feed *movimiento = io.feed("sensorMov");
//feed para sensor magnetico
AdafruitIO_Feed *magnetico = io.feed("sensorMagnetico");



//CODIGO PARA PAGINA WEB
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



////////////////////TERMINO DE CODIGO PARA PAGINA WEB







void setup() {

  //inicializar estado del sistema
  bool estadoSistema = true;

  pinMode(pinAH, INPUT);
  pinMode(pinBP, INPUT);
  pinMode(pinSM, INPUT);
  pinMode(pinSMG, INPUT);
  pinMode(pinLED, OUTPUT);

  WiFi.begin(ssid,password);

  // start the serial connection
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
      { 
        digitalWrite(pinLED, LOW);
        Serial.print(".");
        delay(500);
      }
      Serial.println("");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      digitalWrite(pinLED, HIGH);
      
    
      server.on("/",[](){server.send_P(200,"text/html", webpage);});
      server.on("/state",getState);
      server.begin();

  // wait for serial monitor to open
  while(! Serial);


  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  if(WiFi.status()!=WL_CONNECTED)
      {
          digitalWrite(pinLED, LOW);
          //Serial.print(".");
      }
      else
      {
        digitalWrite(pinLED, HIGH);
        //Serial.println("");
        //Serial.print("IP Address: ");
        //Serial.println(WiFi.localIP());
        server.handleClient();
        getState();
      }


  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  //alarma de humo
  int pinStateAH=digitalRead(pinAH);
  //Serial.print(pinStateAH);
   //boton de panico
  int pinStateBP=digitalRead(pinBP);
  //Serial.print(pinStateBP);
  int pinStateSM = digitalRead(pinSM);
  int pinStateSMG = digitalRead(pinSMG);

  humo->save(pinStateAH);
  boton->save(flagbtn);
  magnetico->save(pinStateSMG);
  movimiento->save(pinStateSM);

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(5000);

}



void getState()
    {

      int pinState_A = digitalRead(pinAH);
      int pinState_P = digitalRead(pinSMG);
      int pinState_B = digitalRead(pinBP);
      int pinState_M = digitalRead(pinSM);
      
      
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
      if(pinState_B == HIGH) {
        flagbtn = 1;
        //btn_state = "\n\t emergency: 1 \n}";
        //Serial.println("High level detected on emergency btn");
      }
      if(flagbtn == 1){
          btn_state = "\n\t emergency: 1 \n}";
      }
       else{
       // flag = 0;
        btn_state = "\n\t emergency: 0 \n}";
        //Serial.println("LOW level detected on emergency btn");
      }

      if(pinState_M == 1){
          motion_state = "\n\t pir: 1, \n";
      }
       else{
       // flag = 0;
        motion_state = "\n\t pir: 0, \n";
        //Serial.println("LOW level detected on emergency btn");
      }

      //motion_state = "\n\t pir: 0, \n";
      //pres_state = "\n \t presence: 1 \n}";-

      total_state = alarm_state+motion_state+pres_state+btn_state;

      server.send(200,"text/plain", total_state);
    }
