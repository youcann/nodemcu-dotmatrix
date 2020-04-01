# nodemcu-dotmatrix
Use a NodeMCU to display the time (via NTP) and the current room temperature/humidity/air pressure (via a BME280) on a dotmatrix display (eight modules, MAX7219)

# Arduino IDE Settings
You need to add the NodeMCU-Board to the Arduino IDE:
File -> Preferences -> Additional Boards Manager URLs -> [http://arduino.esp8266.com/stable/package_esp8266com_index.json!](http://arduino.esp8266.com/stable/package_esp8266com_index.json)

# Needed Libraries
* NTPClient (by Fabrice Weinberg)
* Adafruit Unified Sensor (by Adafruit)
* Adafruit BME280 Library (by Adafruit)
* MD_MAX72XX (by majicDesigns)
* MD_Parola (by majicDesigns)
* PubSubClient (by Nick O'Leary)
