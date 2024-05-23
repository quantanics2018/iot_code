// Define the pin for the built-in LED
const int LED_PIN = 2; // GPIO pin 2 (BUILTIN_LED)

void setup() {
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Turn the LED on
  digitalWrite(LED_PIN, HIGH);
  delay(1000); // Wait for a second

  // Turn the LED off
  digitalWrite(LED_PIN, LOW);
  delay(1000); // Wait for a second
}
