int counter = 0;  // Initialize the integer value to 0
String str;

void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {

  receiveAndSend();
}

// Function to receive serial data and send a response
void receiveAndSend() {
  //Checking is any data is coming from Esp8266
  if (Serial.available()) {
    str = Serial.readString();
    Serial.print("str: ");
    Serial.println(str);
    Serial.flush();//Clearing all Serial print
  }
  //Sending new data to Esp8266
  Serial.print(incrementValue());
  Serial.println(" Hello from Arduino! ");
  delay(1000);
}

// Function to increment the counter and send it through serial communication
int incrementValue() {
  // Increase the integer value by 1
  counter++;

  // Return the updated value
  return counter;
}
