#include <Wire.h>
#define RELAY_PIN 9 // Change D1 to the GPIO pin connected to your relay

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Make sure relay is initially off
}

void loop() {
  // Turn on the relay for 2 seconds
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000); // Wait for 2 seconds

  // Turn off the relay for 2 seconds
  digitalWrite(RELAY_PIN, LOW);
  delay(2000); // Wait for 2 seconds
}
