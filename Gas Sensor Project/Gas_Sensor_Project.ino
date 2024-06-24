#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define RELAY_PIN 23  // GPIO pin where the relay module IN is connected
#define SERVO_PIN 16  // GPIO pin where the servo signal is connected
#define GAS_SENSOR_PIN 36 // Analog input pin for gas sensor
#define buzzerPin 21

// WiFi credentials
const char* ssid = "Quantanics";
const char* password = "Qu@nt@nics18";

// MQTT server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da652102077534423"; // MQTT client ID

Servo myservo;
WiFiClient espClient;
PubSubClient client(espClient);

int gasThreshold = 200;
String relayResponse = "Relay is OFF";
String servoResponse = "Servo is OFF";

void setup() {
  Serial.begin(115200);
  
  // Initialize WiFi connection
  WiFi.begin(ssid, password);

  pinMode(RELAY_PIN, OUTPUT); 
  digitalWrite(RELAY_PIN, LOW);
  myservo.attach(SERVO_PIN);  
  pinMode(buzzerPin, OUTPUT);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  reconnect();
}

void reconnect() {
  // Loop until connected to MQTT broker
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(clientId, mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void publishData(float gas_value, String servo_status, String relay_status) {
  // Create a JSON object
  StaticJsonDocument<200> doc;
  
  // Add data to the JSON object
  doc["Gas Sensor Value"] = gas_value;
  doc["Relay Status"] = relay_status;
  doc["Servo Status"] = servo_status;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Publish the JSON string to the "data" topic
  Serial.println("Publishing data");
  Serial.println(jsonString);
  client.publish("/quantanics/industry/gas_project", jsonString.c_str());
}

void loop() {
  int gasValue = analogRead(GAS_SENSOR_PIN);
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);

  if (gasValue > gasThreshold) {
    
    // Turn the relay on
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(buzzerPin, HIGH);
    relayResponse = "Relay is ON";
    Serial.println("Relay is ON");

    // Rotate the servo motor continuously
    myservo.write(180);  // Command to rotate the continuous rotation servo
    servoResponse = "Servo is ON";
  } else {
    // Turn the relay off
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(buzzerPin, LOW);
    relayResponse = "Relay is OFF";
    Serial.println("Relay is OFF");

    // Stop the servo motor
    myservo.write(90);  // Command to stop the continuous rotation servo (neutral position)
    servoResponse = "Servo is OFF";
  }

  publishData(gasValue, servoResponse, relayResponse);
  delay(1000);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
