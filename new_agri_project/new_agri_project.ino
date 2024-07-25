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
const char* ssid = "Airel_9842878776"; 
const char* password = "air88581"; 
 
// MQTT server settings 
const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883; 
const char* mqtt_topic = "quantanics/industry/testing1"; 
const char* client_id = "2c0134e9-f475-45e0-a175-a9e93bca2366"; 
const char* mqtt_user = "quantanics";   // Add your MQTT username 
const char* mqtt_password = "quantanics"; // Add your MQTT password 
 
// Backend API settings 
const char* api_server = "http://your_backend_server:5000/sensor_data"; 
 
// Pin definitions 
const int soilMoisturePin = 36; // Soil moisture sensor analog pin 
const int smokeSensorPin = 23;   // Smoke sensor analog pin 
const int phSensorPin = 35;      // pH sensor analog pin 
const int relayPin = 17;         // Relay control pin 
 
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
 
void publishData(float temperature, int32_t pressure, float altitude, int soilMoistureValue, int smokeValue, float phValue, bool relayStatus) { 
  StaticJsonDocument<300> doc; 
  doc["temperature"] = temperature; 
  doc["pressure"] = pressure; 
  doc["altitude"] = altitude; 
  doc["soilMoisture"] = soilMoistureValue; 
  doc["smokeValue"] = smokeValue; 
  doc["phValue"] = phValue; 
  doc["relayStatus"] = relayStatus; 
 
  char jsonBuffer[512]; 
  serializeJson(doc, jsonBuffer); 
 
  if (client.publish(mqtt_topic, jsonBuffer)) { 
    Serial.println("Data published successfully"); 
  } else { 
    Serial.println("Failed to publish data"); 
  } 
} 
 
void sendToAPI(float temperature, int32_t pressure, float altitude, int soilMoistureValue, int smokeValue, float phValue, bool relayStatus) { 
  HTTPClient http; 
  http.begin(api_server); 
  http.addHeader("Content-Type", "application/json"); 
 
  StaticJsonDocument<300> doc; 
  doc["temperature"] = temperature; 
  doc["pressure"] = pressure; 
  doc["altitude"] = altitude; 
  doc["soilMoisture"] = soilMoistureValue; 
  doc["smokeValue"] = smokeValue; 
  doc["phValue"] = phValue; 
  doc["relayStatus"] = relayStatus; 
 
  String requestBody; 
  serializeJson(doc, requestBody); 
 
  int httpResponseCode = http.POST(requestBody); 
  if (httpResponseCode > 0) { 
    String response = http.getString(); 
    Serial.println(response); 
  } else { 
    Serial.print("Error on sending POST: "); 
    Serial.println(httpResponseCode); 
  } 
 
  http.end(); 
} 
 
void setup() { 
  Serial.begin(9600); 
   
  pinMode(soilMoisturePin, INPUT); 
  pinMode(smokeSensorPin, INPUT); 
  pinMode(phSensorPin, INPUT); 
  pinMode(relayPin, OUTPUT); 
  digitalWrite(relayPin, LOW); // Ensure the relay is off at startup 
 
  setup_wifi(); 
  client.setServer(mqtt_server, mqtt_port); 
 
  if (!bmp.begin()) { 
    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!"); 
    while (1) {} 
  } 
} 
void loop() { 
  if (!client.connected()) { 
    reconnect(); 
  } 
  client.loop(); 
 
  float temperature = bmp.readTemperature(); 
  int32_t pressure = bmp.readPressure(); 
  float altitude = bmp.readAltitude(); 
  int soilMoistureValue = analogRead(soilMoisturePin); 
  int smokeValue = analogRead(smokeSensorPin); 
  float phValue = analogRead(phSensorPin) * (5.0 / 1023.0) * 3.5; // Example conversion to pH value 
   
  // Validate pH value 
  if (phValue < 0) { 
    phValue = 0; // Ensure pH value is within valid range 
  } 
  if (phValue > 14) { 
    phValue = 14; // Cap the pH value to maximum expected range 
  } 
   
  // Determine relay status based on soil moisture value 
  bool relayStatus = soilMoistureValue < 1000; // Adjust threshold as needed 
  digitalWrite(relayPin, relayStatus ? HIGH : LOW); 
 
  Serial.print("Temperature = "); 
  Serial.print(temperature); 
  Serial.println(" *C"); 
 
  Serial.print("Pressure = "); 
  Serial.print(pressure); 
  Serial.println(" Pa"); 
 
  Serial.print("Altitude = "); 
  Serial.print(altitude); 
  Serial.println(" meters"); 
 
  Serial.print("Soil Moisture Value = "); 
  Serial.println(soilMoistureValue); 
   
  Serial.print("Smoke Sensor Value = "); 
  Serial.println(smokeValue); 
   
  Serial.print("pH Sensor Value = "); 
  Serial.println(phValue); 
 
  Serial.print("Relay Status = "); 
  Serial.println(relayStatus ? "ON" : "OFF"); 
   
  // Send data to backend API 
  sendToAPI(temperature, pressure, altitude, soilMoistureValue, smokeValue, phValue, relayStatus); 
 
  // Publish data to MQTT 
  publishData(temperature, pressure, altitude, soilMoistureValue, smokeValue, phValue, relayStatus); 
 
  Serial.println(); 
  delay(3000); 
}
