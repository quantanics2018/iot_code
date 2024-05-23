const int ledPin = 13;  // Choose the pin for the LED

void setup() {
  pinMode(ledPin, OUTPUT);  // Set the pin as an output
}

void loop() {
  digitalWrite(ledPin, HIGH);  // Turn the LED on
  delay(1000);  // Wait for 1 second
  digitalWrite(ledPin, LOW);  // Turn the LED off
  delay(1000);  // Wait for 1 second
}
