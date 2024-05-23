#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"

uint8_t pinAH = 14; // pin alarma de humo
uint8_t pinBP = 5;  // pin del boton de panico
uint8_t pinSM = 13; // pin del sensor de movimiento
uint8_t pinSMG = 4; // pin del sensor magnetico
uint8_t pinLED = 12; // pin del led indicador

bool sistemaActivo = false;

unsigned long lastReadTime = 0;
const unsigned long readInterval = 5000; // 5 seconds between each reading

// feed para alarma de humo
AdafruitIO_Feed *humo = io.feed("alarmaHumo");
// feed para boton de panico
AdafruitIO_Feed *boton = io.feed("btnPanico");
// feed para sensor de movimiento
AdafruitIO_Feed *movimiento = io.feed("sensorMov");
// feed para sensor magnetico
AdafruitIO_Feed *magnetico = io.feed("sensorMagnetico");

// feed para leer el estado del sistema
AdafruitIO_Feed *estadoSistemaFeed = io.feed("estadoSistema");

void setup() {
  pinMode(pinAH, INPUT);
  pinMode(pinBP, INPUT);
  pinMode(pinSM, INPUT);
  pinMode(pinSMG, INPUT);
  pinMode(pinLED, OUTPUT);

  Serial.begin(115200);
  while(!Serial);
  Serial.print("Connecting to Adafruit IO");

  io.connect();
  estadoSistemaFeed->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  estadoSistemaFeed->get();
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  io.run();
  unsigned long currentTime = millis();
  
  if (currentTime - lastReadTime >= readInterval) {
    estadoSistemaFeed->get();
    lastReadTime = currentTime;
  }

  if (sistemaActivo) {
    int pinStateAH = digitalRead(pinAH);
    int pinStateBP = digitalRead(pinBP);
    int pinStateSM = digitalRead(pinSM);
    int pinStateSMG = digitalRead(pinSMG);

    humo->save(pinStateAH);
    boton->save(pinStateBP);
    movimiento->save(pinStateSM);
    magnetico->save(pinStateSMG);

    digitalWrite(pinLED, HIGH);
  } else {
    Serial.print("The system is OFF");
    digitalWrite(pinLED, LOW);
  }
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.print("estadoSistema <- ");
  Serial.println(data->value());

  int value = data->toInt();
  if(value == 1){
    sistemaActivo = true;
  }
  else{
    sistemaActivo = false;
  }
}
