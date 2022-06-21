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
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Select I2C BUS
void TCA9548A(uint8_t bus){
 Wire.beginTransmission(0x70); // TCA9548A address
 Wire.write(1 << bus); 
 Wire.endTransmission();
}
const char* ssid = "Shiv"; 
const char* pass = "14091999"; 
WiFiClient client;
unsigned long myChannelNumber = 1727324;
const char * myWriteAPIKey = "B33EX8ABC85NY7U7";
float h, tC, tF, p, prin, dp, alt;
float air_quality, air_quality_a;
char temperatureString[6];
char dpString[6];
char altString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme; 
int buzz = 2;
int LED1 = 14;
int LED2 = 12;
int LED3 = 13;
int LED4 = 15;
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
 bme.setHumidityOversampling(BME680_OS_2X);
 bme.setPressureOversampling(BME680_OS_4X);
 bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
 bme.setGasHeater(320, 150);
 
 WiFi.mode(WIFI_STA); 
 
 ThingSpeak.begin(client); 
 
 TCA9548A(2);
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
 Serial.println(F("SSD1306 allocation failed"));
 for(;;);
 } 
 // Clear the buffer
 display.clearDisplay();
 
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
 display.clearDisplay();
 delay(10);
 display.clearDisplay();
 display.setTextSize(2);
display.setTextColor(WHITE);
 display.setCursor(0,0);
 display.println("Project ID : 22303"); 
 display.display();
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("AQI Monitoring System");
 display.display();
 delay(7000);
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
 Serial.println();
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.print("WiFi connected");
 display.display();
 delay(4000);
 pinMode(LED1, OUTPUT);
 pinMode(LED2, OUTPUT);
 pinMode(LED3, OUTPUT);
 pinMode(LED4, OUTPUT);
 pinMode(buzz, OUTPUT);
}
void loop() {
 if(WiFi.status() != WL_CONNECTED){
 Serial.print("Attempting to connect");
 while(WiFi.status() != WL_CONNECTED){
 WiFi.begin(ssid, pass); 
 delay(5000); 
 } 
 Serial.println("\nConnected.");
 }
 MQ135 gasSensor = MQ135(A0);
 
 air_quality_a = analogRead(A0); 
 air_quality = air_quality_a/1;
 TCA9548A(3);
 h = bme.readHumidity();
 tC = bme.readTemperature();
 tF = tC*1.8+32.0;
 dp = tF-0.36*(100.0-h);
 alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
 p = bme.readPressure()/100.0F;
 prin = 0.02953*p;
 
 dtostrf(tC, 5, 1, temperatureString);
 dtostrf(h, 5, 1, humidityString);
 dtostrf(p, 6, 1, pressureString);
 dtostrf(prin, 5, 2, pressureInchString);
 dtostrf(dp, 5, 1, dpString);
 dtostrf(alt, 5, 1, altString);
 
 Serial.print("Air Quality = "); 
 Serial.print(air_quality);
 Serial.println(" PPM");
 Serial.print("Temperature = ");
 Serial.print(temperatureString);
 Serial.println(" °C");
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
 Serial.print("Altitude = ");
 Serial.print(altString);
 Serial.println(" Metres");
 Serial.println();
 
 ThingSpeak.setField(1, air_quality);
 ThingSpeak.setField(2, temperatureString);
 ThingSpeak.setField(3, humidityString);
 ThingSpeak.setField(4, pressureString);
 ThingSpeak.setField(5, pressureInchString);
 ThingSpeak.setField(6, dpString);
 ThingSpeak.setField(7, altString);
 
 int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 if(x==200){
 Serial.println("Channel updated successfuly.");
 Serial.println();
 }
 TCA9548A(2);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Air Quality Index");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(air_quality);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" PPM");
 display.display();
 if (air_quality<=50)
 {
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 delay(1000);
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("Category : Good"); 
 display.display();
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 delay(1000);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Temperature");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(tC);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" deg. C");
 display.display();
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 delay(1000);
 
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Humidity");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(h);
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.println(" %");
 display.display();
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 } 
 
 else if( air_quality>50 && air_quality<=100 )
 {
 
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 delay(1000);
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("Category : Satisfactory"); 
 display.display();
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 delay(1000);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Temperature");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(tC);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" deg. C");
 display.display();
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW);
 delay(1000);
 
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Humidity");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(h);
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.println(" %");
 display.display();
 digitalWrite(LED1, HIGH);
 delay(1500);
 digitalWrite(LED1, LOW); 
 }
 else if( air_quality>100 && air_quality<=200 )
 {
 
 digitalWrite(LED2, HIGH);
 delay(1000);
 digitalWrite(LED2, LOW);
 delay(750);
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("Category : Moderate"); 
 display.display();
 digitalWrite(LED2, HIGH);
 delay(1000);
 digitalWrite(LED2, LOW);
 delay(750);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Temperature");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(tC);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" deg. C");
 display.display();
 digitalWrite(LED2, HIGH);
 delay(1000);
 digitalWrite(LED2, LOW);
 delay(750);
 
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Humidity");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(h);
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.println(" %");
 display.display();
 digitalWrite(LED2, HIGH);
 delay(1000);
 digitalWrite(LED2, LOW); 
 }
 else if( air_quality>200 && air_quality<=300 )
 {
 
 digitalWrite(LED3, HIGH);
 digitalWrite(buzz, HIGH);
 delay(750);
 digitalWrite(LED3, LOW);
 digitalWrite(buzz, LOW);
 delay(750);
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("Category : Poor"); 
 display.display();
 digitalWrite(LED3, HIGH);
 digitalWrite(buzz, HIGH);
 delay(750);
 digitalWrite(LED3, LOW);
 digitalWrite(buzz, LOW);
 delay(750);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Temperature");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(tC);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" deg. C");
 display.display();
 digitalWrite(LED3, HIGH);
 digitalWrite(buzz, HIGH);
 delay(750);
 digitalWrite(LED3, LOW);
 digitalWrite(buzz, LOW);
 delay(750);
 
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Humidity");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(h);
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.println(" %");
 display.display();
 digitalWrite(LED3, HIGH);
 digitalWrite(buzz, HIGH);
 delay(750);
 digitalWrite(LED3, LOW); 
 digitalWrite(buzz, LOW);
 }
 else if( air_quality>300 && air_quality<=400 )
 {
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 delay(500);
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("Category : Severe"); 
 display.display();
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 delay(500);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Temperature");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(tC);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" deg. C");
 display.display();
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 delay(500);
 
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Humidity");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(h);
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.println(" %");
 display.display();
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW); 
 digitalWrite(buzz, LOW); 
 }
 else if (air_quality>400 )
 {
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 delay(350);
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,45);
 display.println("Category : Hazardous"); 
 display.display();
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 delay(350);
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Temperature");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(tC);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println(" deg. C");
 display.display();
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 delay(350);
 
 display.clearDisplay();
 display.setCursor(0,0); 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.println("Humidity");
 
 display.setCursor(0,20); 
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.print(h);
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.println(" %");
 display.display();
 digitalWrite(LED4, HIGH);
 digitalWrite(buzz, HIGH);
 delay(500);
 digitalWrite(LED4, LOW);
 digitalWrite(buzz, LOW);
 }
 
}
