

/* Controllo caldaia Principi Emanuele 2017 
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
//#include <dht11.h>
#include "DHT.h"
//Definizione variabili
#define DHTPIN 1 //definizione del pin input sensore
#define DHTTYPE DHT11   // DHT 11 
//dht11 DHT11;
DHT dht(DHTPIN, DHTTYPE,15);
#define RELEPIN 2 //definizione del pin controllo relay
int tempset ;
int pinData;
int h = 0;
int t = 0;
char auth[] = "0b6e3b4d57914fda8134cf980ef9b809";
SimpleTimer timer;
bool isFirstConnect = true;

//Regolazione temperatura

BLYNK_WRITE(V3) 
{
  int tempset = param.asInt();
}

//Lettura e invio temperatura ambiente

void sendUptime()
{
 // DHT11.read(DHTPIN);
  t = dht.readTemperature();
  Blynk.virtualWrite(10, t);
  h = dht.readHumidity();
  Blynk.virtualWrite(11, h);
}

//Termostato ambiente

void tc()
{
  if ((t >= tempset) & (pinData == 1)) {
    digitalWrite(RELEPIN, false);
    Blynk.virtualWrite (4 , 0);
  }
  else if ((t <= tempset) & (pinData == 1)) {
    digitalWrite(RELEPIN, true);
    Blynk.virtualWrite (4 , 1000);
  }
}

void setup() {
  //  Serial.begin(115200);
  pinMode(RELEPIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  Blynk.begin(auth, "Telecom-71694417", "qqqqqqqq", "emax.tcp4.me");
  timer.setInterval(5000L, sendUptime);
  timer.setInterval(3000L, tc);
}
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    Blynk.syncAll();
    isFirstConnect = false;
  }
}
void loop() {
  Blynk.run();
  timer.run();
}
