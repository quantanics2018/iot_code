#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Quantanics";
const char* password = "Qu@nt@nics18";

// Create an instance of the server
WebServer server(80);

const int trigPin = 16;         // Pin connected to TRIG pin of ultrasonic sensor
const int echoPin = 17;        // Pin connected to ECHO pin of ultrasonic sensor

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
  server.on("/distance", handleDistance);

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Create HTML content with JavaScript for AJAX
  String html = "<html><body><h1>ESP32 Ultrasonic Sensor</h1>";
  html += "<p>Ultrasonic Sensor Distance: <span id=\"distance\">Loading...</span> cm</p>";
  html += "<script>setInterval(function() { "
          "fetch('/distance').then(response => response.text()).then(data => {"
          "document.getElementById('distance').innerHTML = data;"
          "});"
          "}, 1000);</script>";
  html += "</body></html>";

  // Send HTML response
  server.send(200, "text/html", html);
}

void handleDistance() {
  // Measure distance using ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Send distance as plain text
  server.send(200, "text/plain", String(distance));
}
