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
#define  BUF_SIZE  200
char newMessage[BUF_SIZE] = { " " };
char prevMessage[BUF_SIZE] = { " " };
uint8_t scrollSpeed = 35;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 0; // in milliseconds

//----------------------- MQTT -------------------------
const char* mqtt_server = "80.240.20.56";
WiFiClient espClient;
PubSubClient client(espClient);

//mymurks
uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C


//---------------------- BMW280--------------------------
#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#define SEALEVELPRESSURE_HPA (1013.25)
#define SEALEVELPRESSURE_HPA (1038.1)

Adafruit_BME280 bme; // I2C
float bme_temp, bme_humidity, bme_pressure;


//----------------------- NTP ---------------------------
#include <NTPClient.h>
#include <WiFiUdp.h>
const char *ssid     = "T420S 9247";
const char *password = "-e46P913";
const long utcOffsetInSeconds = 3600;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][4] = {"SON", "MON", "DIE", "MIT", "DON", "FRE", "SAM"};
int day;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org",utcOffsetInSeconds);
int ntp_hours, ntp_minutes, ntp_sec;
int counter;
//-------------------------------------------------------



void setup(void)
{
//delay(5000);  //for DEBUG 
//Serial////////////////////////////////
Serial.begin(9600);
while(!Serial); 

//BMW280
unsigned status;
status = bme.begin();  
if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      while (1);
}
Serial.println("Setup");
//DOTMATRIX

P.begin();
P.begin();
P.setIntensity(1);
P.addChar('$', degC);

//WiFi
WiFi.begin(ssid, password);

while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
}
P.displayText("Connected", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  
P.displayAnimate();
delay(500);

//NTP
timeClient.begin();
delay(500);
timeClient.update();
day = timeClient.getDay(); //get current day, 0=Sunday ...
counter = 0;


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
 client.subscribe("#");
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

/*
timeClient.update();
ntp_hours=timeClient.getHours();
ntp_minutes=timeClient.getMinutes();
//ntp_sec=timeClient.getSeconds();
/*
//sprintf(display,"%s %02d:%02d",daysOfTheWeek[day],ntp_hours,ntp_minutes);
sprintf(display,"%s %02d:%02d:%02d",daysOfTheWeek[day],ntp_hours,ntp_minutes,ntp_sec);
P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();
delay(100);
/*
//get sensor values
bme_temp=bme.readTemperature();
bme_humidity=bme.readHumidity();
bme_pressure=bme.readPressure() / 100.0F;

//display sensor values 
//temperature and humidity (5 seconds)
sprintf(display,"%.1f $ %.1f %%",bme_temp,bme_humidity);
P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();
delay(5000);
//pressure (5 seconds)
sprintf(display,"%.1f hPa", bme_pressure);
P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();
delay(5000); */
}

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
    
    static char *cp = newMessage;
    Serial.println("");
    Serial.println("newMessage old: ");
    Serial.println(newMessage);

    Serial.println("");
    Serial.println("prevMessage old: ");
    Serial.println(prevMessage);
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        //msg[i] = (char)payload[i];
        *cp = (char)payload[i];
        cp++;
    }
    *cp = '\0';
    cp = newMessage;

    //check if message was already sent
    if (strcmp(newMessage, prevMessage) != 0){
      strcpy(prevMessage, newMessage);
      //msg[length] = '\0';
      Serial.println();
      
      P.displayText(newMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
      P.displayAnimate();
     }
}

// Get values from sensor and print 
void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
