#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define TRIG_PIN1 D0  // GPIO0 pin connected to the first ultrasonic sensor's trig pin
#define ECHO_PIN1 D7  // GPIO13 pin connected to the first ultrasonic sensor's echo pin

#define TRIG_PIN2 D1  // GPIO14 pin connected to the second ultrasonic sensor's trig pin
#define ECHO_PIN2 D2 // GPIO12 pin connected to the second ultrasonic sensor's echo pin
#define SMOKE_SENSOR_PIN A0  // GPIO4 (A0) pin connected to the smoke sensor

#define DHTPIN 14    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT sensor type (DHT11 or DHT22)
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Airel_9842878776"; // Your WiFi SSID
const char* password = "air88581"; // Your WiFi password

const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttClientID = "259a800e-1ac5-4109-aa6e-3714cbeefa14";
const char* mqttTopic = "Sensor12";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

String receivedData = "";

void setupWiFi() {
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
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqttClientID)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(SMOKE_SENSOR_PIN, INPUT);

  dht.begin();

  setupWiFi();
  client.setServer(mqttServer, mqttPort);

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect
  }
}

void loop() {
  if (Serial.available() > 0) {
    receivedData = Serial.readStringUntil('\n');
    Serial.print("Received Data: ");
    Serial.println(receivedData);
  }

  long duration1, distance1;
  digitalWrite(TRIG_PIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN1, LOW);
  duration1 = pulseIn(ECHO_PIN1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  delay(500);

  long duration2, distance2;
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN2, LOW);
  duration2 = pulseIn(ECHO_PIN2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  delay(500);

  int smokeValue = analogRead(SMOKE_SENSOR_PIN);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.print(" cm, Smoke Value: ");
  Serial.print(smokeValue);

  Serial.println("");

  if (!client.connected()) {
    reconnect();
  }

  String data = String(t) + "," + String(h) + "," + String(distance1) + "," + String(distance2) + "," + String(smokeValue) +"," + String(receivedData);
  client.publish(mqttTopic, data.c_str());

  Serial.println("Published to MQTT: " + data);

  delay(2000);

  if (Serial.available()) {
    Serial.write(Serial.read());
  }
}
