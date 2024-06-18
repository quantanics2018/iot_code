#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> 

// Replace with your network credentials
const char* ssid = "Airel_9842878776";
const char* password = "air88581";

// Create an instance of the server
WebServer server(80);

const int relayPin = 23; // Pin connected to IN pin of relay
bool relayState = LOW;   // Variable to store the relay state

//mqtt username password and server credentials
const char* mqttServer = "broker.emqx.io"; 
const int mqttPort = 1883; 
const char* mqttUser = ""; 
const char* mqttPassword = ""; 
const char* clientId = "db032482-2f63-4ba8-9762-da6521020775"; // MQTT client ID

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayState); // Ensure relay is off at start

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);

  // Start the server
  server.begin();
  Serial.println("Server started");

   client.setServer(mqttServer, mqttPort);
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html>"
                "<html lang='en'>"
                "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>ESP32 Relay Control</title>"
                "<link href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css' rel='stylesheet'>"
                "<style>"
                "body { font-family: Arial, sans-serif; margin: 20px; }"
                "h1 { color: #333; }"
                ".button-container { margin-top: 20px; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='conatiner mt-4 ' style='margin-top:5rem;'>"
                "<div class='row'>"
                "<div class='col-lg-3'></div>"
                "<div class='col-lg-6'>"
                  "<div class='jumbotron border border-2 border-info rounded rounded-o'>"
                    "<h2 class='text-center font-weight-bold text-info m-4'>IOT Door Management System!!</h2>"
                    "<div class='d-flex flex-row w-100 p-2 justify-content-center align-items-center'>"
                      "<button id='door_id' onclick='toggleRelay()' class='btn btn-lg border border-2 border-info text-info rounded'>Open Door</button>"
                    "</div>"
                  "</div>"
                "</div>"
                "<div class='col-lg-3'></div>"
                "</div>"
                "</div>"
                "<script src='https://code.jquery.com/jquery-3.5.1.min.js'></script>"
                "<script>"
                "function toggleRelay() {"
                "  $.get('/toggle', function(data) {"
                "    $('#door_id').text(data)"
                "  });"
                "}"
                "</script>"
                "</body>"
                "</html>";
  server.send(200, "text/html", html);
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

void publishdata(String state,boolean relaystate){
  StaticJsonDocument<100> doc;

  Serial.println("publisher data is");
  Serial.println(relaystate);
  // Add data to the JSON object
  doc["relay_state"] = relaystate;
  doc["door_state"] = state;

   // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  
  
  client.publish("quantanics/industry/project2", jsonString.c_str());
}

void handleToggle() {
  relayState = !relayState; // Toggle the relay state
  digitalWrite(relayPin, relayState);
  Serial.println(relayState);
  String state = relayState ? "Close" : "Open";
  publishdata(state,relayState);
  // mqtt client function checkwifi connection  
  if (!client.connected()) {
        reconnect();
  }
  client.loop();
  server.send(200, "text/plain", "Door " + state);
  
}
