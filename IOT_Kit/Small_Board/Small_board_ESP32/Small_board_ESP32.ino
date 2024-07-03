#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library
#include "DHT.h"

// Constant value declarations
#define TRIGGER_PIN_1 3 // D3
#define ECHO_PIN_1 1    // D1
#define TRIGGER_PIN_2 16 // D16
#define ECHO_PIN_2 17   // D17

#define DHTPIN 23       // Pin for the DHT11 sensor
#define DHTTYPE DHT11   // DHT 11
#define LDR_PIN 36      // Pin for the LDR sensor

#define LCD_COLS 16 // Number of columns on LCD
#define LCD_ROWS 2 // Number of rows on LCD

// WiFi credentials
const char* ssid = "Quantanics1";
const char* password = "Qu@nt@nics18";

// MQTT credentials
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* clientId = "db032482-2f62-4ba8-9782-da6521020775";
const char* mqttTopic = "quantanics/industry/smallBoard";

// Interval for distance measurement and MQTT connection check
unsigned long previousMillis = 0;
const long interval = 1000;

// PubSubClient to get WiFi object
WiFiClient espClient;
PubSubClient client(espClient);

// DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// LCD object
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS); // Address 0x27 for the I2C LCD

void setup() {
  // Initialize WiFi
  WiFi.begin(ssid, password);

  // Initialize sensor pins
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(LDR_PIN, INPUT); // Initialize LDR pin as input

  // Initialize I2C communication
  Wire.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Initialize MQTT
  client.setServer(mqttServer, mqttPort);

  // Initialize DHT sensor
  dht.begin();

  // Display initial message on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

// Reconnect function for MQTT
void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientId, mqttUser, mqttPassword)) {
      // Connected
    } else {
      delay(5000);
    }
  }
}

// Function to publish data to MQTT and update LCD
void publishData(float distance1, float distance2, float temperature, float humidity, int ldrValue) {
  // Create a JSON object
  StaticJsonDocument<200> doc;

  // Add data to the JSON object
  doc["distance1"] = distance1;
  doc["distance2"] = distance2;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["ldrValue"] = ldrValue;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Publish the JSON string to the "data" topic
  client.publish(mqttTopic, jsonString.c_str());

  // Display sensor readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(distance1);
  lcd.print("cm,");
  lcd.print(distance2);
  lcd.print("cm,");
  lcd.print(ldrValue);
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print("C,");
  lcd.print(humidity);
  lcd.print("%");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    long duration1, distance1, duration2, distance2;

    // Read from first ultrasonic sensor
    digitalWrite(TRIGGER_PIN_1, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN_1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN_1, LOW);
    duration1 = pulseIn(ECHO_PIN_1, HIGH);
    distance1 = duration1 * 0.034 / 2;

    // Read from second ultrasonic sensor
    digitalWrite(TRIGGER_PIN_2, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN_2, LOW);
    duration2 = pulseIn(ECHO_PIN_2, HIGH);
    distance2 = duration2 * 0.034 / 2;

    // Read temperature and humidity from DHT11 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Read LDR value
    int ldrValue = digitalRead(LDR_PIN);

    // Publish data from sensors to MQTT topic and update LCD
    publishData(distance1, distance2, temperature, humidity, ldrValue);
  }

  // Check WiFi connection and reconnect if needed
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
