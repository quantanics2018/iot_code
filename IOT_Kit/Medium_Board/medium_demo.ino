#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// Define the pin connected to the soil moisture sensor
const int moistureSensorPin = A0; // Change this to the actual pin you're using

// Define the pin connected to the relay
//const int relayPin = 14; // Change this to the actual pin you're using
#define RELAY_PIN 14

// Define the threshold for activating the relay
const int moistureThreshold = 700; // Change this threshold value as needed

const int ldrpin=16;
const int trigPin = 14;// Trig pin of the ultrasonic sensor
const int echoPin = 12;   // Echo pin of the ultrasonic sensor
int FloatSensor=2;   
const int ledPin2 = 13;  // Change this to the actual pin you're using         
int buttonState = 1; //reads pushbutton status 



const char* ssid = "Airel_9842878776";
const char* password = "air88581";
const char* mqttServer = "broker.emqx.io"; // MQTT broker address
const int mqttPort = 1883; // MQTT broker port
const char* mqttUser = ""; // MQTT username (if required)
const char* mqttPassword = ""; // MQTT password (if required)

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
  
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    pinMode(trigPin, OUTPUT); // Set the trigPin as an OUTPUT
    pinMode(echoPin, INPUT);  // Set the echoPin as an INPUT
   
    pinMode(ledPin2, OUTPUT);
    pinMode(FloatSensor, INPUT_PULLUP); //Arduino Internal Resistor 10K
   

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    client.setServer(mqttServer, mqttPort);
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void loop() {

    // Turn on the relay for 2 seconds
    digitalWrite(RELAY_PIN, HIGH);
    delay(2000); // Wait for 2 seconds
  
    // Turn off the relay for 2 seconds
    digitalWrite(RELAY_PIN, LOW);
    delay(2000); // Wait for 2 seconds
   
    int ldr = digitalRead(ldrpin);
    int moistureLevel = analogRead(moistureSensorPin);
    Serial.print("Moisture level: ");
    Serial.println(moistureLevel);

//    if (moistureLevel < moistureThreshold) {
//        //digitalWrite(relayPin, HIGH);
//        Serial.println("Relay turned ON");
//    } else {
//        //digitalWrite(relayPin, LOW);
//        Serial.println("Relay turned OFF");
//    }

    long duration;
    int distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.print("LDR:");
    Serial.print(ldr);

    

    buttonState = digitalRead(FloatSensor); 
   if (buttonState == LOW) 
   { 
    digitalWrite(ledPin2, HIGH);
    Serial.println( "WATER LEVEL - HIGH"); 
   } 
   else 
   { 
    digitalWrite(ledPin2, HIGH);
    Serial.println( "WATER LEVEL - LOW" ); 
   } 

    if (!client.connected()) {
        reconnect();
    }

    String data = String(distance) + "," + String(buttonState) + "," + String(moistureLevel)+ "," + String(ldr);
    String topic = "Sensors13";
    client.publish(topic.c_str(), data.c_str());
    Serial.println("Published to MQTT: " + data);
    delay(1000);
}
