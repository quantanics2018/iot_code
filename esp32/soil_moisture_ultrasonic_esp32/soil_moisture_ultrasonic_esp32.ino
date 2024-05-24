//library files
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library




// Define the pin where the sensor is connected
const int soilMoisturePin = 36; // Analog pin (GPIO34)
//Ultrasonic sensro pins declaration
#define TRIGGER_PIN 16
#define ECHO_PIN 17


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


  // Initialize the analog pin
  pinMode(soilMoisturePin, INPUT);


//  declare ultrasnoic sensor pin
// Read Input / Output pins  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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
            Serial.println("Connected to MQTT broker");
        }
        else {

            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void publishData(int soil_data,long distance){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["soil_moisture"] = soil_data;
  doc["distance"] = distance;
  

  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  
  client.publish("quantanics/industry/testing1", jsonString.c_str());
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
    
    // Read the analog value from the sensor
    int soilMoistureValue = analogRead(soilMoisturePin);

    
    publishData(soilMoistureValue,distance);
    
    // Print the sensor value to the Serial Monitor
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilMoistureValue);
    Serial.print("Distance");
    Serial.println(distance);
  
    // Wait for a bit before reading again
  }

  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
