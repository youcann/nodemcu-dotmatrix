//--------------------- DOTMATRIX -------------------
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   D7
#define DATA_PIN  D5
#define CS_PIN    D6
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
char display[20];

// ---------------------- WiFi -------------------------
#include <WiFiUdp.h>
const char *ssid     = "";
const char *password = "";


//----------------------- MQTT -------------------------
const char* mqtt_server = "80.240.20.56";
WiFiClient espClient;
PubSubClient client(espClient);
//------------------------------------------------------

void reconnect() {
    while (!client.connected()) {
        Serial.print("Reconnecting...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();
 
    msg[length] = '\0';
    Serial.println(msg);
}



void setup(void)
{
Serial.begin(9600);
while(!Serial); 
Serial.println("Setup");

//DOTMATRIX
P.begin();
P.begin();
P.setIntensity(1);

//WiFi
WiFi.begin(ssid, password);

while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
}
P.displayText("Connected", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);


//MQTT
client.setServer(mqtt_server, 1883);
client.setCallback(callback);

while (!client.connect("baguetteESP")) {
    Serial.println("Connecting to MQTT...");
    if (client.connected()) {
      Serial.println("connected"); 
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
 }
 client.subscribe("covid19");
}



void loop(void){
if (!client.connected()) {
        reconnect();
}
client.loop();
}
