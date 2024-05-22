#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Gas sensor pin declaration
int sensorPin = 36;
int sensorData;

// WiFi credentials
const char* ssid = "Airel_9842878776";
const char* password = "air88581";

// MQTT server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da6521020775"; // MQTT client ID

// PubSubClient to get WiFi object because it needs WiFi status 
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  // WiFi connection initialization  
  WiFi.begin(ssid, password);

  // WiFi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // When WiFi is connected, set server credentials in MQTT  
  Serial.println("Connected to WiFi");
  client.setServer(mqttServer, mqttPort);
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void publishGasSensorData(int gas_sensor_data) {
  // Create a JSON object
  StaticJsonDocument<100> doc;

  // Add gas sensor data to the JSON object
  doc["gas_sensor_data"] = gas_sensor_data;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Publish the JSON string to the "gas_sensor_data" topic
  Serial.println("Publishing gas sensor data");
  Serial.println(jsonString.c_str());
  client.publish("quantanics/industry/gas_sensor_data", jsonString.c_str());
}

void loop() {
  // Read gas sensor data
  sensorData = analogRead(sensorPin);

  // Publish gas sensor data
  publishGasSensorData(sensorData);

  // Check MQTT connection and reconnect if necessary
  if (!client.connected()) {
      reconnect();
  }
  client.loop();

  delay(1000); // Delay for 1 second before reading again
}
