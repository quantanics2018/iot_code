#include "DHT.h"
#include <Wire.h> 
#include <ArduinoJson.h>
#include <BH1750.h>
#include "time.h"  // Add time library

// Network Time Protocol (NTP) settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;    // GMT +5:30 for India (adjust for your timezone)
const int daylightOffset_sec = 0;

// Define the pin where the data line is connected
#define DHTPIN 23  
#define DHTTYPE DHT11

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
#define CONVERSION_FACTOR_SUNLIGHT 55

// Rain gauge variables
const int rainGaugePin = 25;
volatile int rainCounter = 0;
int lastResetHour = -1;  // Track the last reset hour

void IRAM_ATTR countRainfall() {
  rainCounter++;
}

float lux_to_irradiance(float lux, float conversion_factor) {
    return lux / conversion_factor;
}

void checkAndResetRainCounter() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    
    // Reset counter at 6 AM
    if(timeinfo.tm_hour == 6 && lastResetHour != 6) {
        rainCounter = 0;
        lastResetHour = 6;
        Serial.println("Rain counter reset at 6 AM");
    } else if(timeinfo.tm_hour != 6) {
        lastResetHour = timeinfo.tm_hour;  // Update last reset hour
    }
}

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi (Add your WiFi connection code here)
    // WiFi.begin(ssid, password);
    
    // Initialize time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    // Initialize sensors
    dht.begin();
    pinMode(rainGaugePin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(rainGaugePin), countRainfall, FALLING);
    
    Wire.begin(21, 22); // sda , scl
    lightMeter.begin();
}

void publishData(float temperature, float humidity, float rainfall, float irradiation) {
    StaticJsonDocument<100> doc;
    
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["rainfall"] = rainfall;
    doc["irradiation"] = irradiation;
    
    String jsonString;
    serializeJson(doc, jsonString);
    Serial.println(jsonString);
}

void loop() {
    // Check and reset rain counter if needed
    checkAndResetRainCounter();
    
    // Read sensor data
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    float rainfall = rainCounter * 0.2;
    float lux = lightMeter.readLightLevel();
    float irradiance = lux_to_irradiance(lux, CONVERSION_FACTOR_SUNLIGHT);
    
    publishData(temperature, humidity, rainfall, irradiance);
    delay(2000);
}
