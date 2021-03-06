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

//Scrolling
#define  BUF_SIZE  500
char newMessage[BUF_SIZE] = { " " };
char prevMessage[BUF_SIZE] = { " " };
uint8_t scrollSpeed = 70;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 0; // in milliseconds

// ---------------------- WiFi -------------------------
#include <WiFiUdp.h>
const char *ssid     = "";
const char *password = "";


//----------------------- MQTT -------------------------
const char* mqtt_server = "80.240.20.56";
const char* mqtt_client = "";
const char* mqtt_topic = "";
WiFiClient espClient;
PubSubClient client(espClient);
//------------------------------------------------------

void reconnect() {
    while (!client.connected()) {
        Serial.print("Reconnecting...");
        if (!client.connect(mqtt_client)) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    client.subscribe(mqtt_topic);
    Serial.println("mqtt reconnected"); 
    
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    
    static char *cp = newMessage;
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        *cp = (char)payload[i];
        cp++;
    }
    *cp = '\0';
    cp = newMessage;

    //check if new message
    if (strcmp(newMessage, prevMessage) != 0){
      strcpy(prevMessage, newMessage);
      Serial.println();
      P.displayText(newMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
      P.displayAnimate();
     }
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

P.displayText("wifi?", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);
while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
}
P.displayText("wifi!", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);


//MQTT
P.displayText("mqtt?", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);
client.setServer(mqtt_server, 1883);
client.setCallback(callback);

while (!client.connect(mqtt_client)) {
    Serial.println("Connecting to MQTT...");
    if (client.connected()) {
      Serial.println("connected"); 
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
 }
 client.subscribe(mqtt_topic);
 P.displayText("mqtt!", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);

P.displayText("data?", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);
}


void loop(void){
//MQTT
if (!client.connected()) {
        reconnect();
}
client.loop();

//Scrolling
if (P.displayAnimate())
{
    P.displayReset();
}
}
