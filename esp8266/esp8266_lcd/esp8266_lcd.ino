#include <ESP8266WiFi.h>

// Define the analog input pin connected to the current sensor
const int sensorPin = A0;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
}

void loop() {
  // Read the analog voltage from the current sensor
  int sensorValue = analogRead(sensorPin);

  // Convert the sensor value to current (assuming linear relationship)
  // You need to calibrate this conversion based on your sensor's specifications
  float current_mA = map(sensorValue, 0, 1023, 0, 1000); // Assuming 0-1000 mA range

  // Print the current value to the Serial monitor
  Serial.print("Current: ");
  Serial.print(current_mA);
  Serial.println(" mA");

  // Delay before taking the next reading
  delay(1000);
}
