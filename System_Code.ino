#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "Shiv";     // replace with your wifi ssid and wpa2 key
const char *pass = "14091999";
 
WiFiClient client;
 
 
void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  delay(10);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Shiv's Project"); // Print text
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println("AQI Monitoring System");
  display.display();
  delay(4000);
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Connecting to ");
  display.setTextSize(2);
  display.print(ssid);
  display.display();
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    
    display.clearDisplay();
    display.setCursor(0,0);  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("WiFi connected");
    display.display();
    delay(4000);
}
 
 
  void loop()
  {
    MQ135 gasSensor = MQ135(A0);
    float air_quality_a = gasSensor.getPPM();
    float air_quality = air_quality_a;
    Serial.print("Air Quality: ");  
    Serial.print(air_quality);
    Serial.println("  PPM");   
    Serial.println();
 
    display.clearDisplay();
    display.setCursor(0,0);  //oled display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("Air Quality Index");
    
  
    display.setCursor(0,20);  //oled display
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print(air_quality);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(" PPM");
    display.display();

     if (air_quality<=50)
     {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.println("Category : Good"); //Message
      display.display();
      delay(2000);
     } 
     
    else if( air_quality>50 && air_quality<=100 )
     {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.println("Category : Satisfactory"); //Message
      display.display();
      delay(2000);      
     }

     else if( air_quality>100 && air_quality<=200 )
     {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.println("Category : Moderate"); //Message
      display.display();
      delay(2000);      
     }

     else if( air_quality>200 && air_quality<=300 )
     {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.println("Category : Poor"); //Message
      display.display();
      delay(2000);      
     }

     else if( air_quality>300 && air_quality<=400 )
     {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.println("Category : Severe"); //Message
      display.display();
      delay(2000);      
     }

    else if (air_quality>400 )
     {
      
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.println("Hazardous !!!"); //Message
      display.display();
      delay(2000);
     }
 
    delay(2000);      // thingspeak needs minimum 15 sec delay between updates.
}
