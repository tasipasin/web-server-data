#include "DTWebServer.h"

// Replace with your network credentials
const char *ssid = "DuTea-AP";
const char *password = "pucpr2023-tasi*";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
bool stats = false;

/** Objeto do Webserver. */
DTWebServer *webServer;

// Get Sensor Readings and return JSON object
String getSensorReadings() {
  readings["timestamp"] = String(millis());
  readings["relay_state"] = String(stats);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void setup() {
  webServer = DTWebServer(ssid, password);
  // Serial port for debugging purposes
  Serial.begin(115200);
  webServer.init();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    stats = !stats;
    lastTime = millis();
    // Send Events to the client with the Sensor Readings Every 10 seconds
    webServer.send("ping", NULL, millis());
    webServer.send(getSensorReadings().c_str(), "new_readings", millis());
  }
}
