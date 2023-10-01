#ifndef DT_WEB_SERVER_h
#define DT_WEB_SERVER_h

#include "Arduino.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

class DTWebServer
{
public:
    DTWebServer(const char *ssid, const char *password);
    void init(const char *info);
    void sendData(const char *dataToSend, const char *command, long timestamp);

private:
    AsyncWebServer *server;
    AsyncEventSource *events;
    const char *ssid;
    const char *psw;

    void initWiFi();
    void initSPIFFS();
};

#endif
