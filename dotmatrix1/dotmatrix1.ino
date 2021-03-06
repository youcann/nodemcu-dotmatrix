//--------------------- DOTMATRIX -------------------
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   D7
#define DATA_PIN  D5
#define CS_PIN    D6
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
char display[20];

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
const char *ssid     = "";
const char *password = "";
const long utcOffsetInSeconds = 3600;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][4] = {"SON", "MON", "DIE", "MIT", "DON", "FRE", "SAM"};
int day;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org",utcOffsetInSeconds);
int ntp_hours, ntp_minutes, ntp_sec;
int counter;
//-------------------------------------------------------

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



void setup(void)
{
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
}


void loop(void){
//switch between clock and sensors
while(counter < 100){
  timeClient.update();
  ntp_hours=timeClient.getHours();
  ntp_minutes=timeClient.getMinutes();
  ntp_sec=timeClient.getSeconds();

  sprintf(display,"%s %02d:%02d:%02d",daysOfTheWeek[day],ntp_hours,ntp_minutes,ntp_sec);
  P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  counter++;
  delay(100);
}
counter = 0;

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
