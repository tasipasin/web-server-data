#include "DTWebServer.h"

DTWebServer::DTWebServer(const char *ssid, const char *password)
{
  this->ssid = ssid;
  this->psw = password;
}

void DTWebServer::init(const char *info)
{
  this->lastData = info;
  // Determina a porta do servidor
  server = new AsyncWebServer(80);
  events = new AsyncEventSource("/events");
  this->initWiFi();
  this->initSPIFFS();
  // Determina a página raíz da aplicação
  this->server->on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  this->server->serveStatic("/", SPIFFS, "/");

  // Cliente requer os últimos dados ao se conectar
  this->server->on("/readings", HTTP_GET, [this](AsyncWebServerRequest * request)
  {
    request->send(200, "application/json", lastData);
  });
  this->server->addHandler(events);
  this->server->begin();
}

void DTWebServer::sendData(const char *dataToSend, const char *command, long timestamp)
{
  this->lastData = dataToSend;
  this->events->send(dataToSend, command, timestamp);
}

void DTWebServer::initWiFi()
{
  // Inicializa o WIFI como Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(this->ssid, this->psw);
  Serial.println(WiFi.softAPIP());
}

void DTWebServer::initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }
}
