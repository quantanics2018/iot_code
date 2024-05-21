#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

const int moistureSensorPin = A0;
#define RELAY_PIN 14;
const int trigPin = 14;
const int echoPin = 12;
int FloatSensor = 2;
const int ledPin2 = 13;

const char* ssid = "Airel_9842878776";
const char* password = "air88581";
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    Serial.begin(115200);
    WiFi.begin(ssid, password);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin2, OUTPUT);

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
    digitalWrite(RELAY_PIN, HIGH);
    delay(2000);
    digitalWrite(RELAY_PIN, LOW);
    delay(2000);

    int ldr = digitalRead(ldrpin);
    int moistureLevel = analogRead(moistureSensorPin);
    Serial.print("Moisture level: ");
    Serial.println(moistureLevel);

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
    if (buttonState == LOW) {
        digitalWrite(ledPin2, HIGH);
        Serial.println("WATER LEVEL - HIGH");
    } else {
        digitalWrite(ledPin2, HIGH);
        Serial.println("WATER LEVEL - LOW");
    }

    if (!client.connected()) {
        reconnect();
    }

    // Create a JSON object
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["distance"] = distance;
    jsonDoc["moistureLevel"] = moistureLevel;

    // Serialize JSON object to a string
    String data;
    serializeJson(jsonDoc, data);

    String topic = "/quantanics/industry/Medium_Board";
    client.publish(topic.c_str(), data.c_str());
    Serial.println("Published to MQTT: " + data);
    delay(1000);
}
