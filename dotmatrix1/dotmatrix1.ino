//DOTMATRIX/////////////////////////////
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   D7
#define DATA_PIN  D5
#define CS_PIN    D6
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

char display[20];
//mymurks
uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C
////////////////////////////////////////
//BMW280////////////////////////////////
#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#define SEALEVELPRESSURE_HPA (1013.25)
#define SEALEVELPRESSURE_HPA (1038.1)

Adafruit_BME280 bme; // I2C
unsigned long delayTime;
float bme_temp, bme_humidity, bme_pressure;


////////////////////////////////////////
//NTP///////////////////////////////////
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char *ssid     = "Hotpoint";
const char *password = "6161616161";
const long utcOffsetInSeconds = 3600;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][12] = {"SON", "MON", "DIE", "MIT", "DON", "FRE", "SAM"};
long epoch;
long day;
int day_of_week;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int ntp_hours, ntp_minutes, ntp_sec;
int counter;
////////////////////////////////////////

void setup(void)
{
delay(5000); 
//Serial////////////////////////////////
Serial.begin(9600);
while(!Serial); 

//BMW280////////////////////////////////
unsigned status;
status = bme.begin();  
if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      while (1);
}
Serial.println("Setup");

//DOTMATRIX/////////////////////////////

P.begin();
P.begin();
P.setIntensity(1);
P.addChar('$', degC);

//NTP///////////////////////////////////
WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
P.displayText("Connected ", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();
delay(500);
timeClient.begin();
epoch = timeClient.getEpochTime();
day = epoch / 86400L;
day_of_week = day % 7; // 0=Sunday, 1=Monday, ...
counter = 0;
}


void loop(void)
{

while(counter < 100){
////////////////////////////////////////
//NTP///////////////////////////////////
timeClient.update();
ntp_hours=timeClient.getHours();
ntp_minutes=timeClient.getMinutes();
ntp_sec=timeClient.getSeconds();
////////////////////////////////////////
//DOTMATRIX/////////////////////////////
sprintf(display,"%s %02d:%02d:%02d",daysOfTheWeek[day_of_week],ntp_hours,ntp_minutes,ntp_sec);

P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();
counter++;
delay(100);
}
counter = 0;
//BMW280////////////////////////////////
bme_temp=bme.readTemperature();
bme_humidity=bme.readHumidity();
bme_pressure=bme.readPressure() / 100.0F;

sprintf(display,"%.1f $ %.1f %%",bme_temp,bme_humidity);
P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();

delay(5000);
sprintf(display,"%.1f hPa", bme_pressure);
P.displayText(display, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
P.displayAnimate();
delay(5000);
////////////////////////////////////////

}


//DOTMATRIX/////////////////////////////

////////////////////////////////////////
//BMW280////////////////////////////////
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
////////////////////////////////////////
//NTP///////////////////////////////////

////////////////////////////////////////
