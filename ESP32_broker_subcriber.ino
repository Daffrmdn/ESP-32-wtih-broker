// Suscribher 

#include <WiFi.h>
#include "PubSubClient.h"

const char* ssid = "Your wifi name/ssid";
const char* password = "pass wifi";
const char* mqttServer = "broker.hivemq.com"; // i use hivemq for broker
int port = 1883;

char clientId[50];

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
 Serial.begin(115200);

 delay(10);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);

 wifiConnect();

 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());

 client.setServer(mqttServer, port);
 client.setCallback(callback);
}

void wifiConnect() {
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
}

void mqttReconnect() {
 while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   long r = random(1000);
   sprintf(clientId, "clientId-%ld", r);
   if (client.connect(clientId)) {
     Serial.print(clientId);
     Serial.println(" connected");
     client.subscribe("edspert/temp"); // Your name subscribe in broker
     client.subscribe("edspert/hum"); // is the same for different parameter
   } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     delay(5000);
   }
 }
}

void callback(char* topic, byte* message, unsigned int length) {
//  Serial.print("Message arrived on topic: ");
//  Serial.print(topic);
//  Serial.print(". Message: ");
 String stMessage;
  for (int i = 0; i < length; i++) {
   Serial.print((char)message[i]);
   stMessage += (char)message[i];
 }
 Serial.println();
}

void loop() {
 delay(10);
 if (!client.connected()) {
   mqttReconnect();
 }
 client.loop();
}
