//library files
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include "DHT.h"   // DHT library

//constant value declaring
const int sensorPin = 23; // Analog pin connected to the voltage sensor
float voltage;

//wifi username password
const char* ssid = "Airel_9842878776";
const char* password = "air88581";
//mqtt username password and server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da6521020775"; // MQTT client ID

//loop intervelling time initialized value
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check

// pubsubclient to get wifi object because its need wifi status 
WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
  Serial.begin(9600);

  // wifi username and password initializing  
  WiFi.begin(ssid, password);


  // wifi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
//    Serial.println("Connecting to WiFi...");
  }

  // when wifi is connected set server credentials in mqtt  
//  Serial.println("Connected to WiFi");
  client.setServer(mqttServer, mqttPort);

 
}

//its reconnect function
void reconnect() {
    while (!client.connected()) {
//        Serial.println("Attempting MQTT connection...");
        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        }
        else {
//            Serial.print("Failed, rc=");
//            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void publishData(float current_sensor_val){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["current_sensor_value"] = current_sensor_val;
  

  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  // Publish the JSON string to the "data" topic
//  Serial.println("publishing data");
//  Serial.println(jsonString.c_str());
  client.publish("/quantanics/industry/current_sensor1", jsonString.c_str());
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    long duration, distance;
    
    int sensorValue = analogRead(sensorPin); // Read analog value from voltage sensor
    current_sensor = sensorValue * (5.0 / 4095.0); // Convert analog value to voltage (assuming a 5V reference voltage and 12-bit ADC resolution)
      
    Serial.print("Voltage: ");
    Serial.print(current_sensor);
    publishData(current_sensor);
          
  }
  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
