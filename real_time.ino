#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_CCS811.h>  // For CCS811 (Air Quality) sensor (Change if using a different one)
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Sensor setup
Adafruit_CCS811 ccs;

// Server endpoint
String serverUrl = "http://your-server-ip:3000/air-quality";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (!ccs.begin()) {
    Serial.println("Failed to initialize sensor");
    while (1);
  }

  delay(1000);
}

void loop() {
  if (ccs.available()) {
    if (!ccs.readData()) {
      int eco2 = ccs.geteCO2();
      int tvoc = ccs.getTVOC();

      // Send data to the server (eCO2 in ppm)
      sendDataToServer(eco2);
      delay(1000);
    }
  }
}

void sendDataToServer(int eco2) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl); // Specify server URL
    http.addHeader("Content-Type", "application/json"); // Specify content-type header

    // Create JSON body
    String jsonBody = "{\"ppm\": " + String(eco2) + "}";

    // Send HTTP POST request
    int httpResponseCode = http.POST(jsonBody);
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error in sending data");
    }
    http.end(); // Free resources
  } else {
    Serial.println("WiFi Disconnected");
  }
}
