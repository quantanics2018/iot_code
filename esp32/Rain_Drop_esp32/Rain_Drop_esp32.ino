// Define the pin for the raindrop sensor
const int raindropSensorPin = 36; // GPIO pin for raindrop sensor

void setup() {
  Serial.begin(9600);
  pinMode(raindropSensorPin, INPUT);
}

void loop() {
  int raindropSensorValue = analogRead(raindropSensorPin);
  
  // Print the analog value to the Serial Monitor
  Serial.print("Raindrop Sensor Value: ");
  Serial.println(raindropSensorValue);
  
  // Check the sensor value to determine if it's raining
  Serial.println(raindropSensorValue);
  if (raindropSensorValue < 1500) {
    Serial.println("It's raining!");
  } else {
    Serial.println("No rain detected.");
  }
  
  delay(1000); // Delay for a second before checking again
}
