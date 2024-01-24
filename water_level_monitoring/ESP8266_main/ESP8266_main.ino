#include "BlynkConfig.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHT_PIN D3 // Replace with the GPIO pin where the DHT sensor is connected
#define DHT_TYPE DHT11 // DHT sensor type (DHT11 or DHT22)

DHT dht(DHT_PIN, DHT_TYPE);

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
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the pulse duration
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  float distance_cm = duration * 0.034 / 2.0;

  // Check for valid distance readings
  if (distance_cm >= 2 && distance_cm <= 400) {  // Range of reliable measurements
    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    // Update Blynk with the distance value
    Blynk.virtualWrite(V0, distance_cm);

    // Send the distance value as a log to Blynk Terminal (V1)
    Blynk.virtualWrite(V1, "Distance: " + String(distance_cm) + " cm");

    // Additional data to print on the Blynk Terminal
    float temperature = readTemperature();  // Replace with your temperature reading code
    int humidity = readHumidity();          // Replace with your humidity reading code

    Blynk.virtualWrite(V1, "Temperature: " + String(temperature) + " °C");
    Blynk.virtualWrite(V1, "Humidity: " + String(humidity) + " %");
  } else {
    // Invalid reading, possibly out of range or sensor error
    Serial.println("Invalid distance reading");
  }
}



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

float readTemperature() {
  float temperature = dht.readTemperature(); // Read temperature from DHT sensor
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor");
    return -1.0; // Return an error value
  }
  return temperature; // Return the temperature value
}

float readHumidity() {
  float humidity = dht.readHumidity(); // Read humidity from DHT sensor
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity from DHT sensor");
    return -1.0; // Return an error value
  }
  return humidity; // Return the humidity value
}