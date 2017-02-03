
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ThingSpeak.h"
//WiFiClient  client;
#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board
unsigned long myChannelNumber = 77866;
const char * myWriteAPIKey = "AEVF4ZUSWK71HVXF";
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
const char* MY_SSID = "Telecom-71694417"; 
const char* MY_PWD = "qqqqqqqq";
int sent = 0;
void setup() {
  Serial.begin(115200);
  connectWifi();
}

void loop() {
  float temp;
  //char buffer[10];
  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0);
  //String tempC = dtostrf(temp, 4, 1, buffer);//handled in sendTemp()
  Serial.print(String(sent)+" Temperature: ");
  Serial.println(temp);
  
  //if (temp != prevTemp)
  //{
  //sendTeperatureTS(temp);
  //prevTemp = temp;
  //}

   ThingSpeak.writeField(myChannelNumber, 1, temp, myWriteAPIKey);
  delay(60000); // ThingSpeak will only accept updates every 15 seconds.
  
}

void connectWifi()
{
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect





