//library files
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include <Arduino.h>




// Define the pin where the sensor is connected
const int soilMoisturePin = 36; // Analog pin (GPIO34)

//define the rain drop sensor pin
const int raindropSensorPin = 23; // GPIO pin for raindrop sensor

#define RELAY_PIN 19 // Change D1 to the GPIO pin connected to your relay

// ph sensor pin connection
const int Ph_pin = 34; // analog pin connected to ph sensor
float calibration_value = 21.34; // calibration value is determined duration calibration



//wifi username password
const char* ssid = "Airtel-MyWiFi-AMF-311WW-E245";
const char* password = "16a2664f";
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


  // Initialize the analog pin
  pinMode(soilMoisturePin, INPUT);

//  Initialize the analog pin in rain drop sensor
  pinMode(raindropSensorPin, INPUT);

//  relay pin initialization
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Make sure relay is initially off

//  phsensor pin initialization
  pinMode(Ph_pin, INPUT);
 
   // wifi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // when wifi is connected set server credentials in mqtt  
  client.setServer(mqttServer, mqttPort);

 
}



//its reconnect function
void reconnect() {
    while (!client.connected()) {

        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        }
        else {

            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void publishData(int soil_data,int raindropSensorValue,boolean realy_condition,int analogvalue,float voltage_value,float ph_value){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["soil_moisture"] = soil_data;
  doc["raindrop_sensor"] = raindropSensorValue;
  doc["relay_condition"] = realy_condition;
  doc["ph_sensor_analog"] = analogvalue;
  doc["ph_sensor_voltage"] = voltage_value;
  doc["ph_sensor_phvalue"] = ph_value;
  

  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  
  client.publish("quantanics/industry/testing1", jsonString.c_str());
}


void loop() {

   unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    
    // Read the analog value from the sensor
    int soilMoistureValue = analogRead(soilMoisturePin);

      // Map the analog value (0-4095) to a percentage (0-100)
    int soilMoisturePercent = map(soilMoistureValue, 0, 4095, 100, 0);


//    rain drop sesnor value
    int raindropSensorValue = digitalRead(raindropSensorPin);

     Serial.println(raindropSensorValue);


  // php sensor value reading
      int analogValue = analogRead(Ph_pin);
      float voltage = analogValue *(3.3/4095.0); // convert analog value to digital
      float phvalue = 7+((2.5 - voltage) / calibration_value);
      
     
     boolean realy_condition = false;
    if (raindropSensorValue ==1 and soilMoisturePercent<50) {
      realy_condition = true;
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      realy_condition = false;
      digitalWrite(RELAY_PIN, LOW);
    }

    Serial.println(soilMoisturePercent);

    
    publishData(soilMoisturePercent,raindropSensorValue,realy_condition,analogValue,voltage,phvalue);
    
    
    // Print the sensor value to the Serial Monitor
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilMoisturePercent);
  
    // Wait for a bit before reading again
  }

  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
