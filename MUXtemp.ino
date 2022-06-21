#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_BME680.h>
#include <Adafruit_Sensor.h>
#include "MQ135.h"
#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME680 bme;

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

float temp; 
void setup() {

  // Start I2C communication with the Multiplexer
  Wire.begin();

  TCA9548A(3);
  Serial.begin(115200);

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    for(;;);
  }
  bme.setTemperatureOversampling(BME680_OS_8X);

  // Init OLED display on bus number 3
  TCA9548A(2);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();
 
}
 
void loop() {

  TCA9548A(3);
  temp = bme.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" °C");
  delay(1000);

  TCA9548A(2);
  display.setTextColor(WHITE); display.clearDisplay();
  display.setTextSize(2); display.setCursor(30,0); display.print("Temp C");
  display.setTextSize(3); display.setCursor(20,30);
  display.print(temp);
  display.display();
  delay(1000);
  
}
