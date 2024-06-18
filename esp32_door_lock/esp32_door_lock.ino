#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Airel_9842878776";
const char* password = "air88581";

// Create an instance of the server
WebServer server(80);

const int relayPin = 23; // Pin connected to IN pin of relay
bool relayState = LOW;   // Variable to store the relay state

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

void handleToggle() {
  relayState = !relayState; // Toggle the relay state
  digitalWrite(relayPin, relayState);
  Serial.println(relayState);
  String state = relayState ? "Close" : "Open";
  server.send(200, "text/plain", "Door " + state);
}
