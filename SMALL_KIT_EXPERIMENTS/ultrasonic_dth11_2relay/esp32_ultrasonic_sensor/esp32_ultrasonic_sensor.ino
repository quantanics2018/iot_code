#include <Wire.h> 

//constant value declaring

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
    Serial.println(distance);
    if (distance >= 0 && distance <= 400) {
      
     Serial.println(distance);
          
      
    } else {
      Serial.println("Out of range");
    }
  }
}
