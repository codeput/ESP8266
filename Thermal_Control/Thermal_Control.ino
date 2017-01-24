

/*Controllo caldaia 
Principi Emanuele 2017
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <dht11.h>
#define DHTPIN 4 //definizione del pin input sensore
dht11 DHT11;
#define RELEPIN 5 //definizione del pin controllo relay
int tempset ;
float h = 0;
float t = 0;
char auth[] = "0b6e3b4d57914fda8134cf980ef9b809";
SimpleTimer timer;
bool isFirstConnect = true;




  BLYNK_WRITE(V3)
{
  tempset=param.asInt();
//  Serial.println(tempset);
  tc();
}

void sendUptime()
{
  DHT11.read(DHTPIN);
  t = DHT11.temperature - 2 ;
  Blynk.virtualWrite(10, t);
  h = DHT11.humidity ;
  Blynk.virtualWrite(11, h);
}

void tc()
{
  DHT11.read(DHTPIN);
  int ttc = DHT11.temperature - 2 ;
//  Serial.println(ttc);
  if (ttc >= tempset) {;
    digitalWrite(RELEPIN, false);
    Blynk.virtualWrite (4 , 0);
  }
  else if (ttc <= tempset) {
    digitalWrite(RELEPIN, true);
    Blynk.virtualWrite (4 , 1000);
  }
}

void setup() {
//  Serial.begin(115200);
  pinMode(RELEPIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  Blynk.begin(auth, "AP-UP", "qqqqqqqq", "emax.tcp4.me");
  timer.setInterval(3000L, sendUptime);
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

