#include "DTWebServer.h"
#include <Arduino_JSON.h>

/** Define as credenciais da conexão. */
const char *ssid = "DuTea-AP";
const char *password = "pucpr2023-tasi*";

/** Variáveis de controle do processo. */
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
bool stats = false;
bool start = false;

/** Objeto do Webserver. */
DTWebServer *webServer;

/**
   Atualiza o objeto de leituras para enviar por requisição.
*/
String updateSensorReadings() {
  JSONVar readingsJson;
  readingsJson["timestamp"] = String(millis());
  readingsJson["relay_state"] = String(stats);
  return JSON.stringify(readingsJson);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Inicializando");
  webServer = new DTWebServer(ssid, password);
  // Cliente requer os últimos dados ao se conectar
  webServer->getWebServer()->on("/readings", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "application/json", updateSensorReadings());
  });
  webServer->getWebServer()->on("/get-message", HTTP_POST, [](AsyncWebServerRequest * request) {},
  NULL, [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    String jsondata;
    for (size_t i = 0; i < len; i++) {
      jsondata += (char)data[i];
    }
    Serial.print("Post recebido: "); Serial.println(jsondata);
    Serial.println("Dado será tratado");
    JSONVar myObject = JSON.parse(jsondata);
    start = myObject.hasOwnProperty("start") && ((int)myObject["start"]) == 1;
    Serial.println("Dados Tratados");
    request->send(200, "text/plain", "SUCCESS");
  });

  webServer->init();
  Serial.println("Inicializado");
}

void loop() {
  if (start && (millis() - lastTime) > timerDelay) {
    stats = !stats;
    lastTime = millis();
    webServer->sendData(updateSensorReadings().c_str(), "new_readings", millis());
    Serial.print("Alterado estado de "); Serial.print(!stats); Serial.print(" para "); Serial.println(stats);
  }
}
