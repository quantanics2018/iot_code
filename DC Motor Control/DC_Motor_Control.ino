#include <esp32-hal-ledc.h>
#include <esp32-hal.h>
#include <LiquidCrystal_I2C.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

int buttonInput = 5;

int flag = false;

// Define potentiometer pin
int POT_PIN = 36;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

// WiFi credentials
const char* ssid = "Quantanics";
const char* password = "Qu@nt@nics18";

// MQTT server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da6521w345234237534423"; // MQTT client ID

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set the pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // Configure GPIO pin 5 as input with an internal pull-up resistor
  pinMode(buttonInput, INPUT_PULLUP);

  // Set potentiometer pin as input
  pinMode(POT_PIN, INPUT);
  
  // Set up the PWM channel
  ledcSetup(pwmChannel, freq, resolution);

  // Attach the PWM channel to the pin
  ledcAttachPin(enable1Pin, pwmChannel);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // Print initial message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Motor Control");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  Serial.begin(115200);

  // Initialize WiFi connection
  WiFi.begin(ssid, password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  reconnect();
}

void reconnect() {
  // Loop until connected to MQTT broker
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(clientId, mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void publishData(String direction, int speed) {
  // Create a JSON object
  StaticJsonDocument<200> doc;
  
  // Add data to the JSON object
  doc["Direction"] = direction;
  doc["Speed in RPM"] = speed;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Publish the JSON string to the "data" topic
  Serial.println("Publishing data");
  Serial.println(jsonString);
  client.publish("/quantanics/industry/dc_motor_project", jsonString.c_str());
}

void motorMoveForward() {
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  int potValue = analogRead(POT_PIN);
  dutyCycle = find_speed(potValue);
  ledcWrite(pwmChannel, dutyCycle);
  updateLCD("Moving Forward!", dutyCycle);
  delay(2000);
}

void motorMoveBackward() {
  Serial.println("Moving Backward");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  int potValue = analogRead(POT_PIN);
  dutyCycle = find_speed(potValue);
  ledcWrite(pwmChannel, dutyCycle);
  updateLCD("Moving Backward!", dutyCycle);
  delay(2000);
}

void motorStopped() {
  Serial.println("Motor Stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  dutyCycle = 0;
  ledcWrite(pwmChannel, dutyCycle);
  updateLCD("Motor Stopped!", dutyCycle);
  delay(2000);
}

int find_speed(int potValue) {
  return map(potValue, 0, 4095, 0, 255);
}

void updateLCD(String message, int dutyCycle) {
  int maxRPM = 5000;
  int rpm = map(dutyCycle, 0, 100, 0, maxRPM);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(rpm);
  lcd.print("rpm");
  publishData(message, rpm);
}

void loop() {
  // Read the potentiometer value (0-4095 for ESP32 ADC)
  int potValue = analogRead(POT_PIN);

  // Map the potentiometer value to a PWM value (0-255)
  dutyCycle = find_speed(potValue);
  
  // Read the state of the pin
  int buttonState = digitalRead(buttonInput);
  
  if (buttonState == HIGH) {
    if (!flag) {
      motorStopped();
    }
    motorMoveBackward();
    flag = true;
  } else {
    if (flag) {
      motorStopped();
    }
    motorMoveForward();
    flag = false;
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(500);
}
