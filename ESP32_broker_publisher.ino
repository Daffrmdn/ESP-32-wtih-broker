
// Publisher 

#include <WiFi.h>
#include <PubSubClient.h>     // Library for connection ESP32 to MQTT broker
#include <DHT.h>              // Library DHT

#define DHTPIN 22             // Pin DHT
#define DHTTYPE DHT22         // Type DHT

DHT dht(DHTPIN, DHTTYPE);     


const char* ssid = "Your wifi name/ ssid";
const char* password = "";
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

char clientId[50];

WiFiClient wifiClient;                // Membuat objek wifiClient
PubSubClient mqttClient(wifiClient);  // Membuat  objek mqttClient dengan konstruktor objek WiFiClient (Permintaan dari Lib)


// mengatur dan menginisialisasi koneksi ke broker MQTT 
// serta menetapkan fungsi callback yang akan dipanggil 
// ketika pesan diterima oleh klien dari langganan yang dibuat.
void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort); // Mengatur detail broker target  yang digunakan
  mqttClient.setCallback(callback);           // jika kita ingin menerima pesan untuk langganan yang dibuat oleh klien
}

// untuk melakukan koneksi ulang (reconnect) ke broker MQTT 
// jika klien kehilangan koneksi dengan broker 
// atau gagal melakukan koneksi saat pertama kali menjalankan program.
// ESP32 Reconnect to broker
void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
      }      
  }
}

//  fungsi yang akan dipanggil ketika klien menerima pesan dari broker 
//  MQTT yang sesuai dengan langganan yang telah dibuat sebelumnya. 
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
}

void connectToInternet(){
  WiFi.begin(ssid, password);   // Mencoba connect ke Wifi

  // Melakukan pengecekan terhadap status koneksi ke WI-Fi
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    } 
    Serial.println("");
    Serial.println("Connected to Wi-Fi");
}


void setup() {
  Serial.begin(115200);

  dht.begin();  // Deklarasi DHT
  connectToInternet();
  setupMQTT(); // setup koneksi ke broker
}


void loop() {
  //Memastikan koneksi ke MQTT
  if (!mqttClient.connected()){
    reconnect();   // Try to connect with broker
  }
  else{
    // Send the Data
    float temperature = dht.readTemperature();
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);  //Convert float to String
    Serial.print("Temperature: ");
    Serial.println(tempString);
    mqttClient.publish("edspert/temp", tempString);

  
    float humidity = dht.readHumidity();;
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    mqttClient.publish("edspert/hum", humString);
    delay(2000);
  }

}

