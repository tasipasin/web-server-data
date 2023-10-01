#include "DTWebServer.h"
#include <Arduino_JSON.h>

// Replace with your network credentials
const char *ssid = "DuTea-AP";
const char *password = "pucpr2023-tasi*";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
bool stats = false;

const char *readings;

/** Objeto do Webserver. */
DTWebServer *webServer;

// Get Sensor Readings and return JSON object
void updateSensorReadings() {
  JSONVar readingsJson;
  readingsJson["timestamp"] = String(millis());
  readingsJson["relay_state"] = String(stats);
  readings = JSON.stringify(readingsJson).c_str();
}

void setup() {
  webServer = new DTWebServer(ssid, password);
  // Serial port for debugging purposes
  Serial.begin(115200);
  webServer->init(readings);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    stats = !stats;
    lastTime = millis();
    // Send Events to the client with the Sensor Readings Every 10 seconds
    webServer->sendData("ping", NULL, millis());
    updateSensorReadings();
    webServer->sendData(readings, "new_readings", millis());
  }
}
