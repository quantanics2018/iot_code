//library files
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include "DHT.h"   // DHT library

//constant value declaring

//Ultrasonic sensro pins declaration
#define TRIGGER_PIN 16
#define ECHO_PIN 17

// DH11 sensor pins declaration
#define DHTPIN 23    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

//Gas sensor pins declaration
int sensorPin = 36;
int sensorData;

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

//ir sensor pins declaration
int ir_sensor_pin = 22;

//solid moisture pin
int soilMoisturePin = 34;
int moisture_value,sensor_analog;


//waterflow sensor pin declaration
#define LED_BUILTIN 2
#define water_flow_sensor  33


//water flow sensor variable declaration 
long currentMillis_water_flow = 0;
long previousMillis_water_flow = 0;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;


//pulse counter function
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}



//wifi username password
const char* ssid = "Airel_9842878776";
const char* password = "air88581";
//mqtt username password and server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da6521020775"; // MQTT client ID

//loop intervelling time initialized value
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check

// pubsubclient to get wifi object because its need wifi status 
WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
  Serial.begin(9600);

  // wifi username and password initializing  
  WiFi.begin(ssid, password);

  // Read Input / Output pins  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // wifi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
//    Serial.println("Connecting to WiFi...");
  }

  // when wifi is connected set server credentials in mqtt  
//  Serial.println("Connected to WiFi");
  client.setServer(mqttServer, mqttPort);

  // initialize dh11 functions
  dht.begin();

   //  ir sensor initialization function
   pinMode(ir_sensor_pin, INPUT);

   

   // water flow sensor pimode initialization
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(water_flow_sensor, INPUT_PULLUP); 

    // variable initial value declaration
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis_water_flow = 0;

    attachInterrupt(digitalPinToInterrupt(water_flow_sensor), pulseCounter, FALLING);
}

//its reconnect function
void reconnect() {
    while (!client.connected()) {
//        Serial.println("Attempting MQTT connection...");
        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        }
        else {
//            Serial.print("Failed, rc=");
//            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void publishData(float distance,float humidity,float temperature,int gas_sensor_data,int is_sensor_state,int moisture_value,float flowRate,long totalMilliLitres,int total_liter){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["distance"] = distance;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["gas_sensor_data"] = gas_sensor_data;
  doc["IR_sensor"] = is_sensor_state;
  doc["moisture_value"] = moisture_value;
  doc["water_flow_sensor_flow_rate"] = flowRate;
  doc["total_milli_liter"] = totalMilliLitres;
  doc["total_liter"] = total_liter;  
  
  

  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  // Publish the JSON string to the "data" topic
//  Serial.println("publishing data");
//  Serial.println(jsonString.c_str());
  client.publish("quantanics/testing/123", jsonString.c_str());
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    long duration, distance;
    
    digitalWrite(TRIGGER_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIGGER_PIN, LOW);
    
    duration = pulseIn(ECHO_PIN, HIGH);
  
    distance = duration * 0.034 / 2;
    
    if (distance >= 0 && distance <= 400) {
      
       // Read humidity and temperature
      float humidity = dht.readHumidity(); // Store humidity value in variable h
      float temperature = dht.readTemperature(); // Store temperature value in variable t
      sensorData = analogRead(sensorPin);
      int is_sensor_state = digitalRead(ir_sensor_pin);
      sensor_analog = analogRead(soilMoisturePin);
      moisture_value = ( 100 - ( (sensor_analog/4095.00) * 100 ) );

      
      currentMillis_water_flow = millis();
      pulse1Sec = pulseCount;
      pulseCount = 0;
      flowRate = ((1000.0 / (millis() - previousMillis_water_flow)) * pulse1Sec) / calibrationFactor;
      previousMillis_water_flow = millis();
      flowMilliLitres = (flowRate / 60) * 1000;
      totalMilliLitres += flowMilliLitres;

      int total_liter = totalMilliLitres/1000;
      publishData(distance,humidity,temperature,sensorData,is_sensor_state,moisture_value,flowRate,totalMilliLitres,total_liter);
    
      
      
      
    } else {
      Serial.println("Out of range");
    }
  }
  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
