
#include <ESP8266WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library


#define SOIL_MOISTURE_PIN  A0 // Analog pin connected to the soil moisture sensor




//wifi username password
char* ssid = "Airel_9842878776";
char* password = "air88581";
//mqtt username password and server credentials
char* mqttServer = "broker.emqx.io"; 
int mqttPort = 1883; 
char* mqttUser = ""; 
char* mqttPassword = ""; 
char* clientId = "db032482-2f62-6ba8-9799-da6521020775"; // MQTT client ID




//loop intervelling time initialized value
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check

// pubsubclient to get wifi object because its need wifi status 
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  
 Serial.begin(9600);         // Initialize serial communication
  // wifi username and password initializing  
  WiFi.begin(ssid, password);

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
        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("connected wifi");
        }
        else {

            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}



void publishData(int soil_moisture_sensor){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["soli_moisture_sensor"] = soil_moisture_sensor;
  
  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  // Publish the JSON string to the "data" topic
  client.publish("/quantanics/industry/soil_moisture_sensor2", jsonString.c_str());
}



void loop() {
 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int soil_moisture_sensor = analogRead(SOIL_MOISTURE_PIN);  // Read analog value from soil moisture sensor
    Serial.println(soil_moisture_sensor);                      // Print the sensor value
  
    publishData(soil_moisture_sensor);
      
  } 
   if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
