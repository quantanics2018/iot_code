/* 
 * Rui Santos  
 * Complete Project Details http://randomnerdtutorials.com 
*/ 
 
#include <Wire.h> 
#include <Adafruit_BMP085.h> 
#include <WiFi.h> 
#include <PubSubClient.h> 
#include <ArduinoJson.h> 
#include <HTTPClient.h> 
 
// Replace with your network credentials 
const char* ssid = "Quantanics"; 
const char* password = "Quantanics2018"; 
 
// MQTT server settings 
const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883; 
const char* mqtt_topic = "quantanics/industry/agri"; 
const char* client_id = "2c0134e9-f475-45e0-a175-a9e93bca2365"; 
const char* mqtt_user = "";   // Add your MQTT username 
const char* mqtt_password = ""; // Add your MQTT password 
 
// Backend API settings 
const char* api_server = "http://your_backend_server:5000/sensor_data"; 
 
// Pin definitions 
const int soilMoisturePin = 36; // Soil moisture sensor analog pin 
const int smokeSensorPin = 34;   // Smoke sensor analog pin 
//const int phSensorPin = 35;      // pH sensor analog pin 
const int raindrop_pin = 23;
 
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
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 
} 
 
void reconnect() { 
  // Loop until we're reconnected 
  while (!client.connected()) { 
    Serial.print("Attempting MQTT connection..."); 
    // Attempt to connect with username and password 
    if (client.connect(client_id, mqtt_user, mqtt_password)) { 
      Serial.println("connected"); 
      // Subscribe to a topic if needed 
      // client.subscribe(mqtt_topic); 
    } else { 
      Serial.print("failed, rc="); 
      Serial.print(client.state()); 
      Serial.println(" try again in 5 seconds"); 
      // Wait 5 seconds before retrying 
      delay(3000); 
    } 
  } 
} 
 
void publishData(int raindrop, int soilMoistureValue, int smokeValue, float phValue) { 
  StaticJsonDocument<300> doc; 
  //doc["temperature"] = temperature; 
  //doc["altitude"] = altitude; 
  doc["soilMoisture"] = soilMoistureValue; 
  doc["smokeValue"] = smokeValue; 
  doc["phValue"] = phValue; 
  //doc["relayStatus"] = relayStatus; 
  doc["rain_drop"] = raindrop;
 
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
   
  pinMode(soilMoisturePin, INPUT); 
  pinMode(smokeSensorPin, INPUT); 
  //pinMode(phSensorPin, INPUT); 
  //pinMode(relayPin, OUTPUT); 
  //digitalWrite(relayPin, LOW); // Ensure the relay is off at startup 

  pinMode(raindrop_pin,INPUT);
 
  setup_wifi(); 
  client.setServer(mqtt_server, mqtt_port); 
 
//  if (!bmp.begin()) { 
//    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!"); 
//    while (1) {} 
//  } 
} 
void loop() { 
  if (!client.connected()) { 
    reconnect(); 
  } 
  client.loop(); 
  
  int soilMoistureValue = analogRead(soilMoisturePin); 
  int smokeValue = analogRead(smokeSensorPin); 
  //float phValue = analogRead(phSensorPin) * (5.0 / 1023.0) * 3.5; // Example conversion to pH value 
   
  // Validate pH value 
//  if (phValue < 0) { 
//    phValue = 0; // Ensure pH value is within valid range 
//  } 
//  if (phValue > 14) { 
//    phValue = 14; // Cap the pH value to maximum expected range 
//  } 
   
    // Map the analog value (0-4095) to a percentage (0-100)
    int soilMoisturePercent = map(soilMoistureValue, 0, 4095, 100, 0);
    int raindrop = digitalRead(raindrop_pin);

 
 
 
  Serial.print("Smoke Sensor Value = "); 
  Serial.println(smokeValue); 
//   
//  Serial.print("pH Sensor Value = "); 
//  Serial.println(phValue); 
//
  Serial.print("rain drop value");
  Serial.println(raindrop);
 
  //int raindrop = 0;
  //int smokeValue=0;
  int phValue=0;
 
  // Publish data to MQTT 
  publishData(raindrop,  soilMoisturePercent, smokeValue, phValue); 
 
  Serial.println(); 
  delay(3000); 
}
