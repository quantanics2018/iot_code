#include <Wire.h> 
#include "DHT.h"



// Define the pin where the data line is connected
#define DHTPIN 23  
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

//Ultrasonic sensro pins declaration
#define TRIGGER_PIN 17
#define ECHO_PIN 16


//loop intervelling time initialized value  `
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check


void setup() {
  Serial.begin(9600);

  // Read Input / Output pins  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize the DHT sensor
  dht.begin();
 
 
}



void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    long duration, distance;

    
    Serial.print("distance   Value: ");
 
    
    digitalWrite(TRIGGER_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIGGER_PIN, LOW);
    
    duration = pulseIn(ECHO_PIN, HIGH);

    distance = duration * 0.034 / 2;
     // Read humidity (percent)
    float humidity = dht.readHumidity();
    
    // Read temperature in Celsius (isFahrenheit = false)
    float temperature = dht.readTemperature();


     // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
    Serial.println(distance);
    if (distance >= 0 && distance <= 400) {
      
     Serial.println(distance);
        
    } else {
      Serial.println("Out of range");
    }

    // Print the results to the Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
  }
 
}
