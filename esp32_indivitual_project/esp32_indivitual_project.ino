//library files
#include <WiFi.h> // Note the change in WiFi library for ESP32
#include <PubSubClient.h> // kindly import pubsubclient header file in tools
#include <Wire.h> 
#include <ArduinoJson.h> // Include ArduinoJson library
#include <Adafruit_BMP085.h> 





// Define the pin where the sensor is connected
const int soilMoisturePin = 36; // Analog pin (GPIO34)

//define the rain drop sensor pin
const int raindropSensorPin = 23; // GPIO pin for raindrop sensor

//smoke sensor analog connections 
#define MQ7_PIN 34  // Use an ADC1 pin, e.g., GPIO 34

//ph sensor pin
#define PH_PIN 35



//wifi username password
const char* ssid = "Airel_9842878776";
const char* password = "air88581";
//mqtt username password and server credentials
const char* mqttServer = "quantanics.in"; 
const int mqttPort = 1883; 
const char* mqttUser = "quantanics"; 
const char* mqttPassword = "quantanics123"; 
const char* clientId = "db032482-2f62-4ba8-9762-da6521020775"; // MQTT client ID

//loop intervelling time initialized value
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for distance measurement and MQTT connection check



//pressure sensor module initialize
Adafruit_BMP085 bmp; 
// pubsubclient to get wifi object because its need wifi status 
WiFiClient espClient;
PubSubClient client(espClient);




void setup() {
   Serial.begin(9600);

  // wifi username and password initializing  
  WiFi.begin(ssid, password);


  // Initialize the analog pin
  pinMode(soilMoisturePin, INPUT);

//  Initialize the analog pin in rain drop sensor
  pinMode(raindropSensorPin, INPUT);


   // wifi connection status checking 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // when wifi is connected set server credentials in mqtt  
  client.setServer(mqttServer, mqttPort);
  
  if (!bmp.begin()) {  
    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");  
    while (1) {}  
  }

  
 
}



//its reconnect function
void reconnect() {
    while (!client.connected()) {

        if (client.connect(clientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
        }
        else {

            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void publishData(int soil_data,int raindropSensorValue,int analogvalue,float ph_value,int gas_sensor_val,float temperature,int32_t pressure){
  // Create a JSON object
  StaticJsonDocument<100> doc;

  
  // Add data to the JSON object
  doc["soil_moisture"] = soil_data;
  doc["raindrop_sensor"] = raindropSensorValue;
  doc["gas_sensor_value"] = gas_sensor_val;
  doc["ph_analog_value"] = analogvalue;
  doc["ph_value"] = ph_value;
  doc["temperature"] = temperature;
  doc["pressure"] = pressure;
  
  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  
  client.publish("quantanics/industry/agri", jsonString.c_str());
}


void loop() {

   unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    
    // Read the analog value from the sensor
    int soilMoistureValue = analogRead(soilMoisturePin);

    // Map the analog value (0-4095) to a percentage (0-100)
    int soilMoisturePercent = map(soilMoistureValue, 0, 4095, 100, 0);


    // rain drop sesnor value
    int raindropSensorValue = digitalRead(raindropSensorPin);

     
      // php sensor value reading
      // Measure pH value
      float analog_value = analogRead(PH_PIN);
      float phValue = (analogRead(PH_PIN) * (5.0 / 1023.0) * 3.5)-7; // conversion to pH value 

      //  pressure sensor value
      float temperature = bmp.readTemperature();  
      int32_t pressure = bmp.readPressure(); 
       
      // Validate pH value 
      if (phValue < 0) { 
        phValue = 0; // Ensure pH value is within valid range 
      } 
      if (phValue > 14) { 
        phValue = 7; // Cap the pH value to maximum expected range 
      } 
    

     int gas_sensor_val = analogRead(MQ7_PIN);

    Serial.print("Soil moisture sensor percentage value:\t");
    Serial.print(soilMoisturePercent);
    Serial.println("");

    Serial.print("Rain drop sensor value:\t");
    Serial.print(raindropSensorValue);
    Serial.println(""); 

    Serial.print("PH sensor value analog :\t");
    Serial.print(analog_value);
    Serial.print("\t PH value:\t");
    Serial.print(phValue);
    Serial.println(""); 


    Serial.print("Gas sensor value :\t");
    Serial.print(gas_sensor_val); 
    Serial.println("");

    Serial.print("Pressure sensor value is pressure :\t");
    Serial.print(pressure);
    Serial.print("\t Temperature is :\t");
    Serial.print(temperature);
    Serial.println("");

    
    
     
    publishData(soilMoisturePercent,raindropSensorValue,analog_value,phValue,gas_sensor_val,temperature,pressure); 
    // Wait for a bit before reading again
  }

  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
}
