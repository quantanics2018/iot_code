#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Replace these with your network credentials
const char* ssid = "Quantanics1";
const char* password = "Qu@nt@nics18";

// Replace with your MQTT Broker's IP address or hostname
const char* mqtt_server = "broker.emqx.io";

const int relayPin = 32; // Pin connected to IN pin of relay
bool relayState = LOW;   // Variable to store the relay state
bool lastRelayState = LOW; // Variable to store the last relay state

// Initialize the WiFi and MQTT Client objects

WiFiClient espClient;
PubSubClient client(espClient);

// Define a static client ID
const char* client_id = "d60680fb-d9aa-4392-9339-207241b08f2b";

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to Wi-Fi
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

void publishdata(String state, bool relaystate) {
  StaticJsonDocument<100> doc;

  Serial.println("Publishing data:");
  Serial.println(relaystate);
  // Add data to the JSON object
  doc["relay_state"] = relaystate;
  doc["door_state"] = state;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  client.publish("quantanics/door_status", jsonString.c_str());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Parse JSON payload
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, messageTemp);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Extract the "status" value
  int mqttValue = doc["status"];

  Serial.print("Extracted status: ");
  Serial.println(mqttValue);

  // Handle the message received
  if (String(topic) == "vcet/cse/1234") {
    if (mqttValue == 0) { // Open the Lock
      relayState = HIGH; // Toggle the relay state
      digitalWrite(relayPin, relayState);
      Serial.println("Relay state: HIGH (Lock Open)");
    } else { // Close the Lock
      relayState = LOW; // Toggle the relay state
      digitalWrite(relayPin, relayState);
      Serial.println("Relay state: LOW (Lock Closed)");
    }

    // Publish the relay state only if it has changed
    if (relayState != lastRelayState) {
      String state = relayState ? "Lock Open" : "Lock Closed";
      publishdata(state, relayState);
      lastRelayState = relayState; // Update the last relay state
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id)) {
      Serial.println("connected");
      // Subscribe to a topic
      client.subscribe("vcet/cse/1234");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayState); // Ensure relay is off at start
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}