#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "MQ135.h"
#include <Arduino.h>
#include <Wire.h>

const char* ssid = "Shiv";   // your network SSID (name) 
const char* password = "14091999";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1727324;
const char * myWriteAPIKey = "B33EX8ABC85NY7U7";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

float h, t, p, prin, dp, alt;
char temperatureFString[6];
char dpString[6];
char altString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; //BME280 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)

void initBME(){
}

void setup() {
  Serial.begin(115200);  //Initialize serial
  initBME();
  
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    MQ135 gasSensor = MQ135(A0);
      float air_quality;
      float air_quality_a = gasSensor.getPPM(); 
      air_quality = air_quality_a;
      
      h = bme.readHumidity();
      t = bme.readTemperature();
      t = t*1.8+32.0;
      dp = t-0.36*(100.0-h);
      alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
      p = bme.readPressure()/100.0F;
      prin = 0.02953*p;
      
      dtostrf(t, 5, 1, temperatureFString);
      dtostrf(h, 5, 1, humidityString);
      dtostrf(p, 6, 1, pressureString);
      dtostrf(prin, 5, 2, pressureInchString);
      dtostrf(dp, 5, 1, dpString);
      dtostrf(alt, 5, 1, altString);
      
      Serial.print("Air Quality = ");  
      Serial.print(air_quality);
      Serial.println(" PPM");
      Serial.print("Temperature = ");
      Serial.print(temperatureFString);
      Serial.println(" °F");
      Serial.print("Humidity = ");
      Serial.print(humidityString);
      Serial.println(" %");
      Serial.print("Pressure = ");
      Serial.print(pressureString);
      Serial.println(" hPa");
      Serial.print("Pressure Inch = ");
      Serial.println(pressureInchString);
      Serial.print("Dew Point = ");
      Serial.print(dpString);
      Serial.println(" °F");
      Serial.print(altString);
      Serial.println(" Metres");
      Serial.println();

      ThingSpeak.setField(1, air_quality);
      ThingSpeak.setField(2, temperatureFString);
      ThingSpeak.setField(3, humidityString);
      ThingSpeak.setField(4, pressureString);
      ThingSpeak.setField(5, pressureInchString);
      ThingSpeak.setField(6, dpString);
      ThingSpeak.setField(7, altString);
    
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x==200){
        Serial.println("Channel updated successfuly.");
      }
      else{
        Serial.println("Problem in updating channel.");
      }
    
    lastTime = millis();
    Serial.println();
  }
  
}
