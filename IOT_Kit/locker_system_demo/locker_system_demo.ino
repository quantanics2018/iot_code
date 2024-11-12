#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h> 
#include <Wire.h> 

// WiFi credentials
const char* ssid = "Quantanics";
const char* password = "Quantanics2018";

// MQTT Broker
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "quantanics/industry/ultrasonic";


// Ultrasonic sensor 2
#define TRIG_PIN2  17 
#define ECHO_PIN2  16
#define BUZZER_PIN  21
#define LDR_PIN   36  
#define SMOKE_PIN  23

const int thresholdDistance = 30; 
const int ldrThreshold = 2000; 
const int smokeThreshold = 100;

long duration2;
int distance2;
int ldrValue;
int smokeValue;


// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(SMOKE_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
 

  // Connect to WiFi
  setup_wifi();
  
  // Set the MQTT server and callback function
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  


  // Measure distance using the second ultrasonic sensor
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN2, LOW);
  
  duration2 = pulseIn(ECHO_PIN2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  
  // Read the LDR value
  ldrValue = analogRead(LDR_PIN);


  smokeValue = analogRead(SMOKE_PIN);

  if((distance2>30) && !(ldrValue>4090)){
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Buzzer ON");
  }else{
    digitalWrite(BUZZER_PIN, LOW);
     Serial.println("Buzzer OFF");
  }
  
  Serial.print("Distance: ");
  Serial.print(distance2);
  Serial.println("CM");
  Serial.print(" cm, LDR Value: ");
  Serial.println(ldrValue);

  Serial.print("Smoke Value:");
  Serial.println(smokeValue);
 
  
  delay(1000); // Adjust delay as needed
}

void setup_wifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.publish(mqtt_topic, "ESP32 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
