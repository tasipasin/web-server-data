#include "DTWebServer.h"

using namespace std;

DTWebServer::DTWebServer(string ssid, string password)
{
    server(80);
    events("/events");
    this->ssid = ssid;
    this->psw = password;
}

DTWebServer::init()
{
    this.initWiFi();
    this.initSPIFFS();
    // Web Server Root URL
    this->server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(SPIFFS, "/index.html", "text/html"); });

    this->server.serveStatic("/", SPIFFS, "/");

    // Request for the latest sensor readings
    this->server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
                    {
    char *json = getSensorReadings();
    request->send(200, "application/json", json); });
    this->server.addHandler(&events);
    // Start server
    this->server.begin();
}

DTWebServer::send(char *data, char *command, long millis)
{
    this->events.send(data, command, millis);
}

DTWebServer::initWiFi()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->ssid, this->password);
    Serial.println(WiFi.softAPIP());
}
DTWebServer::initSPIFFS()
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