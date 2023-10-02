#include "DTWebServer.h"
#include <Arduino_JSON.h>

/** Define as credenciais da conexão. */
const char *ssid = "DuTea-AP";
const char *password = "pucpr2023-tasi*";

/** Variáveis de controle do processo. */
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
bool stats = false;

/** Objeto do Webserver. */
DTWebServer *webServer;

/**
 * Atualiza o objeto de leituras para enviar por requisição.
 */
String updateSensorReadings() {
  JSONVar readingsJson;
  readingsJson["timestamp"] = String(millis());
  readingsJson["relay_state"] = String(stats);
  return JSON.stringify(readingsJson);
}

void setup() {
  webServer = new DTWebServer(ssid, password);
  // Cliente requer os últimos dados ao se conectar
  webServer->getWebServer()->on("/readings", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "application/json", updateSensorReadings());
  });
  Serial.begin(115200);
  webServer->init();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    stats = !stats;
    lastTime = millis();
    // Envia ping e os novos dados
    webServer->sendData("ping", NULL, millis());
    webServer->sendData(updateSensorReadings().c_str(), "new_readings", millis());
  }
}
