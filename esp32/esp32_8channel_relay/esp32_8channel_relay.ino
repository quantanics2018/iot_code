// Define GPIO pins for the relay channels
const int relayPins[8] = {13,26,27,14};

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set all relay pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    // Ensure all relays are off initially
    digitalWrite(relayPins[i], LOW);
  }
}

void loop() {
  // Turn on each relay one by one with a delay
  for (int i = 0; i < 4; i++) {
    digitalWrite(relayPins[i], HIGH);  // Turn on the relay
    Serial.print("Relay ");
    Serial.print(relayPins[i]);
    Serial.println(" is ON");
    delay(1000);  // Wait for a second
    
    digitalWrite(relayPins[i], LOW);  // Turn off the relay
    Serial.print("Relay ");
    Serial.print(relayPins[i]);
    Serial.println(" is OFF");
    delay(1000);  // Wait for a second
  }
}
