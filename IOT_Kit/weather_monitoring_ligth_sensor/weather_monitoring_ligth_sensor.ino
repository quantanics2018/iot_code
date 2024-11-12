#include "DHT.h"
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include <BH1750.h> // uv sensor library



// Initialize BH1750 sensor
BH1750 lightMeter;
#define CONVERSION_FACTOR_SUNLIGHT 0.0079  // Conversion factor for sunlight (lux to W/m²)


// Define the digital pin
const int digitalPin = 23; // Digital output pin connected to GPIO 23



//wifi username password
const char* ssid = "Quantanics";
const char* password = "Quantanics2018";
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



// Function to convert lux to irradiance (W/m²)
float lux_to_irradiance(float lux, float conversion_factor) {
    return lux * conversion_factor;
}
void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   Wire.begin(21, 22); // SDA, SCL

    Wire.begin(19, 18); // SDA on GPIO19, SCL on GPIO18
   // wifi username and password initializing  
  WiFi.begin(ssid, password);


   pinMode(digitalPin, INPUT);

    // wifi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Check sensor BH1750  
  lightMeter.begin(); 

}





void publishData(float irradiation){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
//  doc["temperature"] = temperature;
//  doc["humidity"] = humidity;
//  doc["rainfall"] = rainfall;
  doc["irradiation"] = irradiation;

  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);
  Serial.println(jsonString);
// client.publish("quantanics/industry/testing1", jsonString.c_str());
}


//its reconnect function
void reconnect() {
    while (!client.connected()) {

        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        }
        else {

            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void loop() {
  // put your main code here, to run repeatedly:
   unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

      
    // Wait a few seconds between measurements
    float lux = lightMeter.readLightLevel();
        // Convert lux to irradiance (W/m²) using the conversion factor for sunlight
    float irradiance = lux_to_irradiance(lux, CONVERSION_FACTOR_SUNLIGHT);

    int digitalValue = digitalRead(digitalPin);

    Serial.print("Rain drop");
    Serial.println(digitalValue);
    Serial.print("Solar irradiance");
    Serial.println(irradiance);
  
    publishData(lux);
  }

}
