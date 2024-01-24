#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Mi 11X";     // Replace with your WiFi SSID
const char* password = "12345678"; // Replace with your WiFi password

const char* serverUrl = "http://192.168.42.123:5000/update"; // Replace with your Flask server URL
const int trigPin = D1;
const int echoPin = D2;

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    Serial.print("Wi-Fi Status: ");
    Serial.println(WiFi.status());
    Serial.print("Server URL: ");
    Serial.println(serverUrl);
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  // Check WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverUrl); // Updated line
    http.addHeader("Content-Type", "application/json");

    http.setTimeout(10000); // Set timeout to 10 seconds


    // Prepare JSON data string
    String httpRequestData = "{\"level\": " + String(distance) + "}";
    
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(http.getString());
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Serial.print("Wi-Fi Status: ");
      Serial.println(WiFi.status());
      Serial.print("Server URL: ");
      Serial.println(serverUrl);

    }

    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // Send data every 10 seconds
}
