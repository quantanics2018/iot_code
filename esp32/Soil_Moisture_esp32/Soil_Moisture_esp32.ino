#include <Wire.h> 
#define SOIL_MOISTURE_PIN A0 //Analog pin connected to the soil moisture sensor

//loop intervelling time initialized value
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check


void setup() {
  
 Serial.begin(9600);         // Initialize serial communication
}


void loop() {
 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int soil_moisture_sensor = analogRead(SOIL_MOISTURE_PIN);  // Read analog value from soil moisture sensor
    Serial.println(soil_moisture_sensor);   
    if(soil_moisture_sensor>2700){
      Serial.print("No Moiture");
      }// Print the sensor value
      else{
        Serial.print("Moiture Present");
        }
 
      
  } 
}
