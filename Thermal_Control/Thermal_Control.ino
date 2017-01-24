

//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <dht11.h>
#define DHTPIN 4 //pin gpio 12 in sensor
dht11 DHT11;
#define FANPIN 5
bool tcbool = true;
int tempset ;
//DHT dht(DHTPIN, DHTTYPE);
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
    digitalWrite(FANPIN, false);
    Blynk.virtualWrite (4 , 0);
  }
  else if (ttc <= tempset) {
    digitalWrite(FANPIN, true);
    Blynk.virtualWrite (4 , 1000);
  }
}

void setup() {
//  Serial.begin(115200);
  pinMode(FANPIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  Blynk.begin(auth, "Atlantica_Ospiti", "atlantica123ospiti", "emax.tcp4.me");
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

