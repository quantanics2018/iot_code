#include <SoftwareSerial.h>
int counter = 100;  // Initialize the integer value to 0
String str;

SoftwareSerial espSerial(4, 0);  // (D2 = 4) RX, (D3 = 0) TX pins for ESP8266 (you can use any available pins)

void setup() {
  Serial.begin(9600);     // Initialize the Arduino serial port
  espSerial.begin(9600);  // Initialize the ESP8266 serial port
}

void loop() {
  receiveAndSend();
}

// Function to receive serial data and send a response
void receiveAndSend() {

  if (espSerial.available()) {
    str = espSerial.readString();
    Serial.print("esp: ");
    Serial.println(str);
    espSerial.flush();
  }
  // Send a response back through serial communication
  espSerial.print(incrementValue());
  espSerial.println(" Hello from Esp! ");
  delay(3000);
}


// Function to increment the counter and send it through serial communication
int incrementValue() {
  // Increase the integer value by 1
  counter++;

  // Return the updated value
  return counter;
}
