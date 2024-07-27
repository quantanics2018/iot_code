#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


// WiFi and MQTT credentials
const char* ssid = "Quantanics1234";
const char* password = "Qu@nt@nics18";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqttTopic = "waterlevel";
const char* mqtt_username = ""; // Add your MQTT username
const char* mqtt_password = ""; // Add your MQTT password
const char* mqtt_client_id = "5b42fd6a-cc80-452c-9c35-c37110c0d5d1"; // Add your MQTT client ID

// Pin definitions
#define TRIG_PIN 16
#define ECHO_PIN 17
#define TANK_CAPACITY_CM 100 // Adjust this based on your tank capacity
#define PH_PIN 36
#define ONE_WIRE_BUS 23
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define Offset 0.04  



// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// Objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED, OUTPUT);
  Serial.println("pH meter experiment!");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize MQTT client
  client.setServer(mqtt_server, mqtt_port);
  reconnect();

  sensors.begin();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publish_Data(float ph_value, float temperature, int water_percentage, String water_status) {
  StaticJsonDocument<200> doc;
  doc["water_percentage"] = water_percentage;
  doc["ph_value"] = ph_value;
  doc["temperature"] = temperature;
  doc["water_status"] = water_status;

  String jsonString;
  serializeJson(doc, jsonString);
  // Publish the JSON string to the "data" topic
  client.publish(mqttTopic, jsonString.c_str());
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Measure water level
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration_us = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = 0.0343 * duration_us / 2.0;

  int water_distance_percentage = 0;
  String water_level_status = "";
  if (distance_cm <= TANK_CAPACITY_CM) {
    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    // Check water level and send alert if below 20%
    float percentage_full = (distance_cm / TANK_CAPACITY_CM) * 100.0;
    float reverse_percentage = 100.0 - percentage_full;
    Serial.print("Water level (%): ");
    Serial.println(reverse_percentage);

    if (reverse_percentage <= 15.0 && reverse_percentage >= 0.0) {
      water_level_status = "ALERT !! Water Tank was dry!!";
    } else if (reverse_percentage <= 45.0 && reverse_percentage > 15.0) {
      water_level_status = " Water below Medium";
    } else if (reverse_percentage <= 50.0 && reverse_percentage > 45.0) {
      water_level_status = " Water Tank is Medium";
    } else if (reverse_percentage < 95.0 && reverse_percentage > 50.0) {
      water_level_status = " Water above Medium";
    } else if (reverse_percentage <= 100.0 && reverse_percentage >= 95.0) {
      water_level_status = "ALERT !! Water Tank Full";
    }
    water_distance_percentage = reverse_percentage;
    Serial.print("Water status : ");
    Serial.println(water_level_status);

  } else {
    Serial.println("Water status : tank level exceeded");
    water_distance_percentage > 100;
    water_level_status = "tank level exceeded";
  }

  // Measure pH value
  float phValue = (analogRead(PH_PIN) * (5.0 / 1023.0) * 3.5)-7; // conversion to pH value 
   
  // Validate pH value 
  if (phValue < 0) { 
    phValue = 0; // Ensure pH value is within valid range 
  } 
  if (phValue > 14) { 
    phValue = 7; // Cap the pH value to maximum expected range 
  } 

  // Read temperature using DS18B20 sensor
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Publish data to MQTT
  publish_Data(phValue, temperature, water_distance_percentage, water_level_status);

  delay(50); // Wait for 1 second before taking the next measurement
}
