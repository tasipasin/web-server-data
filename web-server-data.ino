#include "DTWebServer.h"
#include <Arduino_JSON.h>

/** Define as credenciais da conexão. */
const char *ssid = "DuTea-AP";
const char *password = "pucpr2023-tasi*";

/** Variáveis de controle do processo. */
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
bool stats = false;

const char *readings;

/** Objeto do Webserver. */
DTWebServer *webServer;

/**
 * Atualiza o objeto de leituras para enviar por requisição.
 */
void updateSensorReadings() {
  JSONVar readingsJson;
  readingsJson["timestamp"] = String(millis());
  readingsJson["relay_state"] = String(stats);
  readings = JSON.stringify(readingsJson).c_str();
}

void setup() {
  webServer = new DTWebServer(ssid, password);
  Serial.begin(115200);
  updateSensorReadings();
  webServer->init(readings);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    stats = !stats;
    lastTime = millis();
    // Envia ping e os novos dados
    webServer->sendData("ping", NULL, millis());
    updateSensorReadings();
    webServer->sendData(readings, "new_readings", millis());
  }
}
