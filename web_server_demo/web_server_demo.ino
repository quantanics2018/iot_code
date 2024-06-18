#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Quantanics";
const char* password = "Qu@nt@nics18";

// Create an instance of the server
WebServer server(80);

const int relayPin = 23; // Pin connected to IN pin of relay

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure relay is off at start

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
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><body><h1>ESP32 Relay Control</h1>"
                "<p><a href=\"/on\"><button>Door Open</button></a></p>"
                "<p><a href=\"/off\"><button>Door Close</button></a></p>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(relayPin, HIGH);
  server.send(200, "text/html", "<html><body><h1>Relay is ON</h1><br><a href=\"/\"><button>Go Back</button></a></body></html>");
}

void handleOff() {
  digitalWrite(relayPin, LOW);
  server.send(200, "text/html", "<html><body><h1>Relay is OFF</h1><br><a href=\"/\"><button>Go Back</button></a></body></html>");
}
