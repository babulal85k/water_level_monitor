#include "BlynkConfig.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk credentials



// Ultrasonic Sensor Pins
const int trigPin = D1;
const int echoPin = D2;

// Default password for other networks
const char* defaultPass = "12345678";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Scan for Wi-Fi networks and connect
  scanAndConnect();

  // Initialize Blynk - assumes WiFi is connected
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());

  // Initialize Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    reconnectWiFi();
  }

  if (Blynk.connected()) {
    Blynk.run();
  }

  measureDistance();
}

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  Blynk.virtualWrite(V0, distance);
}

// ... [measureDistance function as before]

void scanAndConnect() {
  while (true) {
    Serial.println("Scanning for Wi-Fi networks...");
    int numNetworks = WiFi.scanNetworks();

    if (numNetworks == 0) {
      Serial.println("No networks found");
    } else {
      for (int i = 0; i < numNetworks; ++i) {
        String ssid = WiFi.SSID(i);
        Serial.print("Trying ");
        Serial.println(ssid);

        // Use the default password for all networks
        WiFi.begin(ssid.c_str(), defaultPass);

        int counter = 0;
        while (WiFi.status() != WL_CONNECTED && counter < 20) {
          delay(250);
          Serial.print(".");
          counter++;
        }

        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("\nConnected to network: " + ssid);
          return; // Exit the function once connected
        } else {
          Serial.println("\nConnection to network " + ssid + " Failed");
        }
      }
    }

    Serial.println("Could not connect to any network, retrying...");
    delay(5000); // Wait for 5 seconds before scanning again
  }
}


void reconnectWiFi() {
  WiFi.disconnect();
  delay(1000); // Wait for a while to ensure disconnection
  scanAndConnect();

  if (WiFi.status() == WL_CONNECTED) {
    Blynk.disconnect(); // Ensure Blynk is disconnected before reinitializing
    Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());
  } else {
    Serial.println("Failed to reconnect to any Wi-Fi network");
  }
}