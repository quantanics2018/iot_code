#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// Water Flow Sensor
#define FlowSensor_INPUT 2 // Arduino interrupt pin connected to flow sensor
#define FLOW_RATE_CONSTANT 7.5 // Flow rate constant for the sensor
#define PERIOD 1000 // Time period in milliseconds to calculate flow rate

volatile unsigned long pulse_counter = 0;
unsigned long old_time;

#define LDR_SENSOR_PIN 11  // GPIO pin connected to the LDR sensor
#define Soil_SENSOR_PIN A1
#define Rain_SENSOR_PIN A0
const int relayPin = 9;
SoftwareSerial espSerial(0, 1);
Servo servo;

String str;

String obstacleState = "no obstacle";

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  pinMode(LDR_SENSOR_PIN, INPUT);
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // Water Flow Sensor
  pinMode(FlowSensor_INPUT, INPUT);
  attachInterrupt(digitalPinToInterrupt(FlowSensor_INPUT), interrupt_handler, RISING);
  old_time = millis();

  // Servo motor
  servo.attach(10); // Connect the servo signal wire to pin 3
}

void loop() {
  int LDRSensorValue = digitalRead(LDR_SENSOR_PIN);
  int soilValue = analogRead(Soil_SENSOR_PIN);
  int rainValue = analogRead(Rain_SENSOR_PIN);
  float flow_rate = 0;

  digitalWrite(relayPin, HIGH);
  delay(3000); // Wait for 3 seconds

  // Turn off the relay
  digitalWrite(relayPin, LOW);
  delay(3000); // Wait for 3 seconds

  // Water Flow Sensor
  unsigned long new_time = millis() - old_time;

  if (new_time >= PERIOD) {
    noInterrupts();
    unsigned long pulse_count = pulse_counter;
    pulse_counter = 0;
    interrupts();

    old_time = millis();

    float pulse_rate = (pulse_count * 1000.0) / new_time; // Convert ms to seconds
    flow_rate = pulse_rate * FLOW_RATE_CONSTANT;
  }

  // Servo motor control
  for (int angle = 10; angle < 180; angle++) {
    servo.write(angle);
    delay(15);
  }
  for (int angle = 180; angle > 10; angle--) {
    servo.write(angle);
    delay(15);
  }

  Serial.print("LDR:");
  Serial.print(LDRSensorValue);
  Serial.print(",");
  Serial.print("SOIL:");
  Serial.print(soilValue);
  Serial.print(",");
  Serial.print("RAIN:");
  Serial.print(rainValue);
  Serial.print(",");
  Serial.print("FLOWRATE:");
  Serial.println(flow_rate, 2);
  Serial.println("Liter/hour");

  str = String(LDRSensorValue) + ","  + String(soilValue) + "," + String(rainValue) + ","  + String(flow_rate);
  espSerial.println(str);

  delay(2000);
}

void interrupt_handler() {
  pulse_counter++;
}
