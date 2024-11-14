#include <WiFi.h> 
#include <PubSubClient.h> 
#include <ArduinoJson.h>
#include <Wire.h> 


#define ANALOG_IN_PIN 34 // Define the analog pin
#define BUZZER_PIN  21


// Replace with your network credentials 
const char* ssid = "Quantanics"; 
const char* password = "Quantanics2018"; 
 
// MQTT server settings 
const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883; 
const char* mqtt_topic = "quantanics/industry/voltage"; 
const char* client_id = "2c0134e9-f475-45e0-a175-a9e93bca2363"; 
const char* mqtt_user = "";   // Add your MQTT username 
const char* mqtt_password = ""; // Add your MQTT password 



// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
    Serial.begin(115200); // Initialize serial communication
    pinMode(BUZZER_PIN, OUTPUT);

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



void publishData(int voltage, String buzzer_status) { 
  StaticJsonDocument<300> doc; 
  
  doc["voltage"] = voltage; 
  doc["buzzer_status"] = buzzer_status;  

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
  
    int adcValue = analogRead(ANALOG_IN_PIN); // Read ADC value
    float voltage = adcValue * (16.5 / 4095); // Convert ADC value to actual voltage
    String buzzer_status = "OFF";
    if(voltage>10){
       digitalWrite(BUZZER_PIN, HIGH);
       buzzer_status="ON";
    }else{
       digitalWrite(BUZZER_PIN, LOW);
       buzzer_status="OFF";
    }
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

    publishData(voltage, buzzer_status);
    delay(500); // Delay for half a second
}
