#include <WiFi.h> 
#include <PubSubClient.h> 
#include <ArduinoJson.h>
#include <Wire.h> 

#define IR_SENSOR_PIN 23
#define LDR_PIN 36

// Replace with your network credentials 
const char* ssid = "Quantanics"; 
const char* password = "Quantanics2018"; 
 
// MQTT server settings 
const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883; 
const char* mqtt_topic = "quantanics/industry/street_light"; 
const char* client_id = "2c0134e9-f475-45e0-a175-a9e93bca2363"; 
const char* mqtt_user = "";   // Add your MQTT username 
const char* mqtt_password = ""; // Add your MQTT password 
const int relayPin= 13;

WiFiClient espClient; 
PubSubClient client(espClient); 

void setup() {
  Serial.begin(115200);
  pinMode(IR_SENSOR_PIN, INPUT);  // Set IR sensor pin as input
  pinMode(LDR_PIN, INPUT); // Set LDR sensor pin as input
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Ensure all relays start in the OFF position
 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("Connecting to Wi-Fi..."); 
  } 
  Serial.println("\nWiFi connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

  // Set MQTT server and port
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() { 
  while (!client.connected()) { 
    Serial.print("Attempting MQTT connection..."); 
    if (client.connect(client_id, mqtt_user, mqtt_password)) { 
      Serial.println("Connected to MQTT server"); 
    } else { 
      Serial.print("Failed to connect, rc="); 
      Serial.print(client.state()); 
      Serial.println(" trying again in 5 seconds"); 
      delay(5000); 
    } 
  } 
}

void publishData(int ldr, int ir, String light_status) { 
  StaticJsonDocument<300> doc; 
  
  doc["ldr_value"] = ldr; 
  doc["ir_value"] = ir; 
  doc["light_status"] = light_status; 

  char jsonBuffer[512]; 
  serializeJson(doc, jsonBuffer); 

  if (client.publish(mqtt_topic, jsonBuffer)) { 
    Serial.println("Data published successfully"); 
  } else { 
    Serial.print("Failed to publish data. MQTT state: ");
    Serial.println(client.state()); // Print MQTT state for debugging
  } 
} 

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int ir_Value = digitalRead(IR_SENSOR_PIN);  // Read the value from the IR sensor
  int ldrValue = analogRead(LDR_PIN);
  
  Serial.print("IR sensor value: ");
  Serial.println(ir_Value);  
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  String light_status = "OFF";
  
  // Control logic for light
  if (ldrValue < 10) { // Adjust threshold as necessary
    if (ir_Value == 1) {
      digitalWrite(relayPin, LOW); // Light ON
      light_status = "ON";
    } else {
      digitalWrite(relayPin, HIGH); // Light OFF
      light_status = "OFF";
    }
  } else {
    digitalWrite(relayPin, HIGH); // Light OFF in bright ambient light
    light_status = "OFF";
  }

  publishData(ldrValue, ir_Value, light_status);
  delay(1000); // Wait for 1 second before the next iteration
}
