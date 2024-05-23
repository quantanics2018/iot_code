// Define the pin for the IR sensor
const int irSensorPin = 13; // GPIO pin for IR sensor

void setup() {
  Serial.begin(9600);
  pinMode(irSensorPin, INPUT);
}

void loop() {
  int irSensorValue = digitalRead(irSensorPin);
  
  if (irSensorValue == HIGH) {
    Serial.println("No Object detected!");
  } else {
    Serial.println("object detected.");
  }
  
  delay(500); // Delay for half a second before checking again
}
