#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// WiFi credentials
#define WLAN_SSID       "redmi_ap"
#define WLAN_PASS       "PCUAQ2024"

// Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "MarianaGrad09"
#define AIO_KEY         
// Create an ESP8266 WiFiClient class to connect to the MQTT server
WiFiClient client;

// Create an Adafruit_MQTT_Client instance
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup a feed subscription
Adafruit_MQTT_Subscribe feed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/estadosistema");
uint8_t pinAH = 14; // pin alarma de humo
uint8_t pinBP = 5;  // pin del boton de panico
uint8_t pinSM = 13; // pin del sensor de movimiento
uint8_t pinSMG = 4; // pin del sensor magnetico
uint8_t pinLED = 12; // pin del led indicador

// Setup feeds for publishing data
Adafruit_MQTT_Publish humo = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/alarmaHumo");
Adafruit_MQTT_Publish boton = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/btnPanico");
Adafruit_MQTT_Publish movimiento = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensorMov");
Adafruit_MQTT_Publish magnetico = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensorMagnetico");

String value;

void setup() {
    pinMode(pinAH, INPUT);
    pinMode(pinBP, INPUT_PULLUP);
    pinMode(pinSM, INPUT);
    pinMode(pinSMG, INPUT_PULLUP);
    pinMode(pinLED, OUTPUT);

    Serial.begin(115200);
    delay(10);

    // Connect to WiFi
    Serial.println(); Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");

    // Set up MQTT subscription for the feed
    mqtt.subscribe(&feed);
}

void loop() {
    // Ensure the connection to the MQTT server is alive
    MQTT_connect();

    // Check if new data is available
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) {
        if (subscription == &feed) {
            Serial.print("Received: ");
            value = (char *)feed.lastread;
            Serial.println(value);
        }
    }

    if (value == "1") {
        int pinStateAH = digitalRead(pinAH);
        int pinStateBP = digitalRead(pinBP);
        int pinStateSM = digitalRead(pinSM);
        int pinStateSMG = digitalRead(pinSMG);

        

        Serial.print(pinStateAH);
        Serial.print("\n");
        Serial.print(pinStateBP);
        Serial.print("\n");
        Serial.print(pinStateSM);
        Serial.print("\n");
        Serial.print(pinStateSMG);
        Serial.print("\n");
        Serial.print("fin");


        humo.publish(pinStateAH);
        boton.publish(pinStateBP);
        movimiento.publish(pinStateSM);
        magnetico.publish(pinStateSMG);

        digitalWrite(pinLED, HIGH);
    } else if (value == "0") {
        Serial.print("The system is OFF\n");
        digitalWrite(pinLED, LOW);
    }
}

// Function to connect and reconnect as necessary to the MQTT server
void MQTT_connect() {
    int8_t ret;

    // Stop if already connected
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for success
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(10);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            // Reset the device or take some other action
            while (1);
        }
    }
    Serial.println("MQTT Connected!");
}
