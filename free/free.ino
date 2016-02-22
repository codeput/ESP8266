//Mqtt relay board control for ESP8266-01  
//This sketch is usefull for DIoTY Android APP
//Emanuele Principi emaxduck@gmail.com

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Wi-Fi setup
const char *ssid =	"<Your_ssid>";
const char *pass =	"<Your_wifi_password>";

//DIoTY topic configuration
String rtopic = "<Your_root_topic>";
String topic1 = "<Your_subtopic_1>";
String topic2 = "<Your_subtopic_2>";
String device  = "<Your_device_uid>";


void callback(const MQTT::Publish& pub) {

  String istruzione = "";
  Serial.print(pub.topic());
  Serial.print(" => ");
  istruzione = pub.payload_string();
  Serial.println(istruzione);

  if ((pub.topic () == topic1) && (istruzione == "true")) {
    digitalWrite(2, HIGH);
    Serial.println("GPIO2 ON");
  } else if ((pub.topic () == topic1) && (istruzione == "false")) {
    digitalWrite(2, LOW);
    Serial.println("GPIO2 OFF");
  }


  if ((pub.topic () == topic2) && (istruzione == "true")) {
    digitalWrite(0, HIGH);
    Serial.println("GPIO0  ON");
  } else if ((pub.topic () == topic2) && (istruzione == "false")) {
    digitalWrite(0, LOW);
    Serial.println("GPIO0 OFF");
  }

}

WiFiClient wclient;

//mqtt DioTY account
String mqttServerName = "<mqtt.dioty.co>";
int    mqttport = 1883;
String mqttuser =  "<your_DIoTY_account>";
String mqttpass =  "<your_DIoTY_password>";

PubSubClient client(wclient, mqttServerName, mqttport);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  
  // Setup I/O port
  pinMode(2, OUTPUT);
  pinMode(0, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect(device)
                         .set_auth(mqttuser, mqttpass))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.publish(rtopic, "hello world");
        client.subscribe(topic1);
        client.subscribe(topic2);

      } else {
        Serial.println("Could not connect to MQTT server");
      }
    }

    if (client.connected())
      client.loop();
  }
}

