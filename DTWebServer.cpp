#include "DTWebServer.h"

DTWebServer::DTWebServer(const char *ssid, const char *password)
{
  this->ssid = ssid;
  this->psw = password;
}

void DTWebServer::init(const char *info)
{
  server = new AsyncWebServer(80);
  events = new AsyncEventSource("/events");
  this->initWiFi();
  this->initSPIFFS();
  // Web Server Root URL
  this->server->on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  this->server->serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  this->server->on("/readings", HTTP_GET, [&info](AsyncWebServerRequest * request)
  {
    request->send(200, "application/json", info);
  });
  this->server->addHandler(events);
  // Start server
  this->server->begin();
}

void DTWebServer::sendData(const char *dataToSend, const char *command, long timestamp)
{
  this->events->send(dataToSend, command, timestamp);
}

void DTWebServer::initWiFi()
{
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
