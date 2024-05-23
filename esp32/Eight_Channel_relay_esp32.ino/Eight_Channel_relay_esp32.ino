#include <Wire.h>

#define RELAY_PIN_0 D0
#define RELAY_PIN_1 D1
#define RELAY_PIN_2 D2
#define RELAY_PIN_3 D3
#define RELAY_PIN_4 D4
#define RELAY_PIN_5 D5
#define RELAY_PIN_6 D6
#define RELAY_PIN_7 D7
 
void setup() {
  pinMode(RELAY_PIN_0, OUTPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(RELAY_PIN_5, OUTPUT);
  pinMode(RELAY_PIN_6, OUTPUT);
  pinMode(RELAY_PIN_7, OUTPUT);
  
  digitalWrite(RELAY_PIN_0, LOW);  // Make sure relay is initially off
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);
  digitalWrite(RELAY_PIN_3, LOW);
  digitalWrite(RELAY_PIN_4, LOW);
  digitalWrite(RELAY_PIN_5, LOW);
  digitalWrite(RELAY_PIN_6, LOW);
  digitalWrite(RELAY_PIN_7, LOW);
}

void loop() {
  // Turn on the relay for 2 seconds
  digitalWrite(RELAY_PIN_0, HIGH);
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);
  digitalWrite(RELAY_PIN_4, HIGH);
  digitalWrite(RELAY_PIN_5, HIGH);
  digitalWrite(RELAY_PIN_6, HIGH);
  digitalWrite(RELAY_PIN_7, HIGH);
  delay(2000); // Wait for 2 seconds

  // Turn off the relay for 2 seconds
  digitalWrite(RELAY_PIN_0, LOW);
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);
  digitalWrite(RELAY_PIN_3, LOW);
  digitalWrite(RELAY_PIN_4, LOW);
  digitalWrite(RELAY_PIN_5, LOW);
  digitalWrite(RELAY_PIN_6, LOW);
  digitalWrite(RELAY_PIN_7, LOW);
  delay(2000); // Wait for 2 seconds
}
