#include <Wire.h>  
#include <Adafruit_BMP085.h>  
#include <WiFi.h>  
#include <PubSubClient.h>  
#include <ArduinoJson.h>  
#include "DHT.h"


#define DHTPIN 23     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
  
// Replace with your network credentials  
const char* ssid = "Airel_9842878776";  
const char* password = "air88581";  
  
// MQTT server settings  
const char* mqtt_server = "broker.emqx.io";  
const int mqtt_port = 1883;  
const char* mqtt_topic = "quantanics/industry/weather_data";  
const char* client_id = "2c0134e9-f475-45e0-a175-a9e93bca2366";  
const char* mqtt_user = "";   // Add your MQTT username  
const char* mqtt_password = ""; // Add your MQTT password  
  

// Define the pin for the raindrop sensor
const int raindropSensorPin = 36; // GPIO pin for raindrop sensor

  
Adafruit_BMP085 bmp;  
WiFiClient espClient;  
PubSubClient client(espClient);  
  
void setup_wifi() {  
  delay(10);  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  
  WiFi.begin(ssid, password);  
  
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);  
    Serial.print(".");  
  }  
  
  Serial.println("");  
  Serial.println("WiFi connected");  

}  
  
void reconnect() {  
  // Loop until we're reconnected  
  while (!client.connected()) {  
    Serial.print("Attempting MQTT connection...");  
    if (client.connect(client_id, mqtt_user, mqtt_password)) {  
      Serial.println("connected");  
    } else {  
      Serial.print("failed, rc=");  
      Serial.print(client.state());  
      Serial.println(" try again in 5 seconds");  
      delay(3000);  
    }  
  }  
}  
  
void publishData(float temperature, int32_t pressure,int raindropSensorValue,int dht11_humidity, int dht11_temperature) {  
  StaticJsonDocument<200> doc;  
  doc["temperature"] = temperature;  
  doc["pressure"] = pressure;  
  doc["rain_drop_value"] = raindropSensorValue;

  doc["dht11_temperature"] = dht11_temperature;
  doc["dht11_humidity"] = dht11_humidity;
  
  char jsonBuffer[512];  
  serializeJson(doc, jsonBuffer);  
  
  if (client.publish(mqtt_topic, jsonBuffer)) {  
    Serial.println("Data published successfully");  
  } else {  
    Serial.println("Failed to publish data");  
  }  
}  
  
  
void setup() {  
  Serial.begin(9600);  
    
  setup_wifi();  
  client.setServer(mqtt_server, mqtt_port);  
  pinMode(raindropSensorPin, INPUT);
  if (!bmp.begin()) {  
    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");  
    while (1) {}  
  }

  dht.begin();
}  
  
void loop() {  
  if (!client.connected()) {  
    reconnect();  
  }  
  client.loop();  

//  pressure sensor value
  float temperature = bmp.readTemperature();  
  int32_t pressure = bmp.readPressure();  

//  DHT11 SENSOR value
  float dht11_humidity = dht.readHumidity();
  float dht11_temperature = dht.readTemperature();

  int raindropSensorValue = analogRead(raindropSensorPin);
  if (raindropSensorValue < 1500) {
    Serial.println("It's raining!");
  } else {
    Serial.println("No rain detected.");
  }
  Serial.print("Temperature = ");  
  Serial.print(temperature);  
  Serial.println(" *C");  
  
  Serial.print("Pressure = ");  
  Serial.print(pressure);  
  Serial.println(" Pa"); 


  Serial.print("Humidity: ");
  Serial.print(dht11_humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(dht11_temperature);
  Serial.println(" *C");

 
  // Publish data to MQTT  
  publishData(temperature, pressure,raindropSensorValue,dht11_humidity,dht11_temperature);  
 
  Serial.println();  
  delay(1000);  
}
