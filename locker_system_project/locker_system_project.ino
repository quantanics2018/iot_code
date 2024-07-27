Locker System
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "realme 12 Pro 5G";
const char* password = "a6b9nien";

// MQTT Broker
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "quantanics/industry/ultrasonic";

// DHT11 sensor
#define DHTPIN 23    // Pin to which the DHT11 sensor is connected
#define DHTTYPE DHT11 // Define sensor type as DHT11
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic sensor 1
#define TRIG_PIN  17  // Ultrasonic sensor TRIG pin
#define ECHO_PIN  16  // Ultrasonic sensor ECHO pin

// Ultrasonic sensor 2
#define TRIG_PIN2  1 // Second Ultrasonic sensor TRIG pin
#define ECHO_PIN2  3 // Second Ultrasonic sensor ECHO pin

#define LDR_PIN   36  // LDR pin (analog input)
#define BUZZER_PIN  21 // Buzzer pin
#define SMOKE_PIN  23 // Smoke sensor analog output pin

const int thresholdDistance = 30; // 40 cm threshold distance
const int ldrThreshold = 2000; // LDR threshold value (adjust based on your setup)
const int smokeThreshold = 100; // Smoke sensor threshold value (adjust based on your setup)

long duration;
int distance;
long duration2;
int distance2;
int ldrValue;
int smokeValue;
float temperature;
float humidity;

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SMOKE_PIN, INPUT);
  
  digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off initially

  // Initialize DHT11 sensor
  dht.begin();

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
  
  // Measure distance using the first ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

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

  // Read the smoke sensor value
  smokeValue = analogRead(SMOKE_PIN);

  // Read temperature and humidity from DHT11 sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check if the object is within 40 cm, if LDR value exceeds the threshold, or if smoke is detected
  if (distance < thresholdDistance || ldrValue > ldrThreshold || smokeValue > smokeThreshold) {
    digitalWrite(BUZZER_PIN, HIGH); // Sound the alarm
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Turn off the alarm
  }
  
  // Create a JSON formatted string for the data
  String payload = "{\"distance1\":";
  payload += distance;
  payload += ", \"distance2\":";
  payload += distance2;
  payload += ", \"ldr_value\":";
  payload += ldrValue;
  payload += ", \"smoke_value\":";
  payload += smokeValue;
  payload += ", \"temperature\":";
  payload += temperature;
  payload += ", \"humidity\":";
  payload += humidity;
  payload += "}";
  
  // Publish the data to the MQTT topic
  client.publish(mqtt_topic, payload.c_str());
  
  Serial.print("Distance 1: ");
  Serial.print(distance);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.print(" cm, LDR Value: ");
  Serial.print(ldrValue);
  Serial.print(", Smoke Value: ");
  Serial.print(smokeValue);
  Serial.print(", Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  delay(1000); // Adjust delay as needed
}

void setup_wifi() {
  delay(10);
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
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(mqtt_topic, "ESP32 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
