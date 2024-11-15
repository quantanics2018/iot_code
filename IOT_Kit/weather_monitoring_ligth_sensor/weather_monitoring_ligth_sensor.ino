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
const char* mqtt_topic = "quantanics/industry/weather_monitoring";


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


   pinMode(digitalPin, INPUT);

   // Connect to Wi-Fi
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) { 
      delay(500); 
      Serial.print("Connecting to Wi-Fi..."); 
    } 
    Serial.println("\nWiFi connected"); 
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP());
  
    
  // Check sensor BH1750  
  lightMeter.begin();
  // Set MQTT server and port
  client.setServer(mqttServer, mqttPort); 

}





void publishData(int rain_drop_value,int irradiance){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["irradiation"] = irradiance;
  doc["rain_drop_val"] = rain_drop_value;

  
  // Serialize the JSON object to a string
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  if (client.publish(mqtt_topic, jsonBuffer)) { 
    Serial.println("Data published successfully"); 
  } else { 
    Serial.print("Failed to publish data. MQTT state: ");
    Serial.println(client.state()); // Print MQTT state for debugging
  } 
}


void reconnect() { 
  while (!client.connected()) { 
    Serial.print("Attempting MQTT connection..."); 
    if (client.connect(clientId, mqttUser, mqttPassword)) { 
      Serial.println("Connected to MQTT server"); 
    } else { 
      Serial.print("Failed to connect, rc="); 
      Serial.print(client.state()); 
      Serial.println(" trying again in 5 seconds"); 
      delay(5000); 
    } 
  } 
}



void loop() {
  // put your main code here, to run repeatedly:
   unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    // Wait a few seconds between measurements
    float lux = lightMeter.readLightLevel();
        // Convert lux to irradiance (W/m²) using the conversion factor for sunlight
    float irradiance = lux_to_irradiance(lux, CONVERSION_FACTOR_SUNLIGHT);

    int digitalValue = digitalRead(digitalPin);

    Serial.print("Rain drop");
    Serial.println(digitalValue);
    Serial.print("Solar irradiance");
    Serial.println(irradiance);
  
    publishData(digitalValue,irradiance);
  }

}
