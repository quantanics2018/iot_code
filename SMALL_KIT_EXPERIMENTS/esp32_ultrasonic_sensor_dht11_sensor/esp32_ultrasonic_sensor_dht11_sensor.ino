//library files
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include "DHT.h"


//constant value declaring

//Ultrasonic sensro pins declaration
#define TRIGGER_PIN 17
#define ECHO_PIN 16

// Define the pin where the data line is connected
#define DHTPIN 23  
#define DHTTYPE DHT11



// two channel relay pin 
const int relayPin1 = 27;  
const int relayPin2 = 26;

//wifi username password
const char* ssid = "BECORE Academy";
const char* password = "Password@0";
//mqtt username password and server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da6521020795"; // MQTT client ID

//loop intervelling time initialized value
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check

// pubsubclient to get wifi object because its need wifi status 
WiFiClient espClient;
PubSubClient client(espClient);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);

  // wifi username and password initializing  
  WiFi.begin(ssid, password);

  // Read Input / Output pins  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

 
  // Initialize the DHT sensor
  dht.begin();

  
  // wifi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // when wifi is connected set server credentials in mqtt  
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


void publishData(float distance,float humidity_data,float temp_data){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["distance"] = distance;
  doc["temperature_data"] = temp_data;
  doc["humidity_data"] = humidity_data;  

  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  // Publish the JSON string to the "data" topic
//  Serial.println("publishing data");
//  Serial.println(jsonString.c_str());
  client.publish("quantanics/industry/ultrasonic_dht11_sensor", jsonString.c_str());
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    long duration, distance;
    
    digitalWrite(TRIGGER_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIGGER_PIN, LOW);
    
    duration = pulseIn(ECHO_PIN, HIGH);
  
    distance = duration * 0.034 / 2;

     // Read humidity (percent)
    float humidity = dht.readHumidity();
  
    // Read temperature in Celsius (isFahrenheit = false)
    float temperature = dht.readTemperature();


    Serial.println(distance);
    if (distance >= 0 && distance <= 400) {
      
       // Read humidity and temperature
     Serial.print("Distance data:\t");
     Serial.println(distance);
     Serial.print("DHT11 Humidity data:\t");
     Serial.print(humidity);
      Serial.println("%");

     Serial.print("DHT11 Temperature data:\t");
     Serial.print(temperature);
     Serial.println("C");
     publishData(distance,humidity,temperature);
          
    
    } else {
      Serial.println("Out of range");
    }
  }
  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
