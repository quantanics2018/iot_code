#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>


//Ultrasonic sensro pins declaration
#define TRIGGER_PIN 17
#define ECHO_PIN 16
#define TANK_CAPACITY_CM 100 // Adjust this based on your tank capacity


// Create an instance of the sensor
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);



//wifi username password
const char* ssid = "Quantanics";
const char* password = "Quantanics2018";
//mqtt username password and server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f62-4ba8-9782-da6521020775"; // MQTT client ID

//loop intervelling time initialized value  `
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
  }


  if (!bmp.begin()) {
    Serial.print("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  // when wifi is connected set server credentials in mqtt  
  client.setServer(mqttServer, mqttPort);

 
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


void publishData(float water_percentage,String water_status,float pressure,float temperature){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["water_level"] = water_percentage;
  doc["water_status"] = water_status;
  doc["pressure"] = pressure;
  doc["temperature"] = temperature;
  
 
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);
  client.publish("industry/ultrasonic1", jsonString.c_str());
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


    sensors_event_t event;
    bmp.getEvent(&event);
    
    int water_distance_percentage = 0;
    String water_level_status = "";
    Serial.println(distance);
    if (distance >= 0 && distance <=TANK_CAPACITY_CM) {

      // Check water level and send alert if below 20%
      float percentage_full = (distance / TANK_CAPACITY_CM) * 100.0;
      float reverse_percentage = 100.0 - percentage_full;

       if (reverse_percentage <= 15.0 && reverse_percentage >= 0.0) {
        water_level_status = "ALERT !! Water Tank was dry!!";
      } else if (reverse_percentage <= 45.0 && reverse_percentage > 15.0) {
        water_level_status = " Water below Medium";
      } else if (reverse_percentage <= 50.0 && reverse_percentage > 45.0) {
        water_level_status = " Water Tank is Medium";
      } else if (reverse_percentage < 95.0 && reverse_percentage > 50.0) {
        water_level_status = " Water above Medium";
      } else if (reverse_percentage <= 100.0 && reverse_percentage >= 95.0) {
        water_level_status = "ALERT !! Water Tank Full";
      }
      water_distance_percentage = reverse_percentage;  
      
    } else {
       Serial.println("Water status : tank level exceeded");
       //water_distance_percentage > 100;
       water_level_status = "tank level exceeded";
    }

    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Pressure: ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

      
    Serial.print("Water Percentage");
    Serial.println(water_distance_percentage);
    publishData(water_distance_percentage, water_level_status,event.pressure,temperature);
  }
  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
