#ifndef DT_WEB_SERVER_h
#define DT_WEB_SERVER_h

#include "Arduino.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

class DTWebServer
{
public:
    DTWebServer();
    void init();
    void send(char *data, char *command, long millis);

private:
    AsyncWebServer server;
    AsyncEventSource events;
    JSONVar readings;
    char *ssid;
    char *psw;

    void initWiFi();
    void initSPIFFS();
}

#endif
