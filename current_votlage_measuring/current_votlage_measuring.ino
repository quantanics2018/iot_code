#include <WiFi.h>
#include <PubSubClient.h>

#include <ArduinoJson.h> 
#include <Wire.h>


//adjust accordingly
const char* mqtt_topic = "quantanics/industry/sensor_datatesttt4";
const float currentSensorSensitivity = 38.938 ; // mV/A (adjust for your sensor)
 
// Replace with your network credentials 

const char* ssid = "Airel_9842878776"; 
const char* password = "air88581"; 
 
// MQTT server settings 
const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883; 
 
const char* client_id = "2c0134e9-f475-45e0-a175-a9e93bca2365"; 
const char* mqtt_user = "";   // Add your MQTT username 
const char* mqtt_password = ""; // Add your MQTT password 
 
// Backend API settings 
const char* api_server = "http://your_backend_server:5000/sensor_data"; 

// ADC Pins
const int voltageSensorPin = 33;
const int currentSensorPin = 34;

// ADC and Sensor Characteristics
const float adcResolution = 4096.0; // 12-bit ADC resolution
const float referenceVoltage = 3.3; // Reference voltage of ESP32 ADC (in volts)
const float voltageStepDownFactor = 16.5 / 4095; // Voltage sensor factor


// Sensor Offset (calibration)variable
float currentSensorOffset = 0;
 

 
WiFiClient espClient; 
PubSubClient client(espClient); 


float calibrateCurrentSensor() {
  const int samples = 500;
  long totalAdc = 0;
  for (int i = 0; i < samples; i++) {
    totalAdc += analogRead(currentSensorPin);
    delay(10);
  }
  return ((totalAdc / samples) / adcResolution) * referenceVoltage;
}


float readVoltage() {
  int adcValue = analogRead(voltageSensorPin);
  return adcValue * voltageStepDownFactor;
}


float readCurrent() {
  const int samples = 200;
  long totalAdc = 0;
  for (int i = 0; i < samples; i++) {
    totalAdc += analogRead(currentSensorPin);
    delay(5);
  }
  float averageAdc = totalAdc / samples;
  float voltage = (averageAdc / adcResolution) * referenceVoltage;
  return (voltage - currentSensorOffset) / (currentSensorSensitivity / 1000);
}
 
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
       client.subscribe(mqtt_topic); 
    } else { 
      Serial.print("failed, rc="); 
      Serial.print(client.state()); 
      Serial.println(" try again in 5 seconds"); 
      // Wait 5 seconds before retrying 
      delay(3000); 
    } 
  } 
} 
 
void publishData(float voltage, float current){
  StaticJsonDocument<300> doc; 
  doc["voltage"] = voltage; 
  doc["current"] = current; 
  

  char jsonBuffer[512]; 
  serializeJson(doc, jsonBuffer); 
  if (client.publish(mqtt_topic, jsonBuffer)) { 
    Serial.println("Data published successfully"); 
  } else { 
    Serial.println("Failed to publish data"); 
  } 
  
} 
 
 
void setup() { 
  Serial.begin(115200); 
   
  pinMode(33, INPUT);
  pinMode(34, INPUT); 
   
  
 
  setup_wifi(); 
  client.setServer(mqtt_server, mqtt_port); 
  currentSensorOffset = calibrateCurrentSensor();

} 
void loop() { 
  if (!client.connected()) { 
    reconnect(); 
  } 
  client.loop(); 


  
  float voltage = 0.0;
  float t;
  for(int x=1 ; x <= 50 ; x++){
    t = readVoltage();
    voltage = voltage + t;
    delay(5);
  }
  voltage = (voltage/50.0);
  if(voltage <= 0.5) voltage = 0.0;
  float current = readCurrent();
  if(current <= 0.05)  current = 0.0;

  // Publish data
  publishData(voltage, current);
  Serial.print(voltage);
  Serial.print("   ");
  Serial.println(current);

  delay(750);
}
