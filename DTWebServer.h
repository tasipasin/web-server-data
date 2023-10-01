#ifndef DT_WEB_SERVER_h
#define DT_WEB_SERVER_h

#include "Arduino.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

/**
 * Classe de Controle do Servidor WEB.
 */
class DTWebServer
{
public:
  /**
   * Classe de Controle do Servidor WEB.
   * @param ssid Nome da Rede.
   * @param password Senha da Rede.
   */
    DTWebServer(const char *ssid, const char *password);
    
    /**
     * Iniciador do Web Server.
     * @param info Referência para os dados que devem ser enviados ao conectar o servidor.
     */
    void init(const char *info);

    /**
     * Envia os dados para o Web Server.
     * @param dataToSend Dados formatados como JSON para envio.
     * @param command Comando do envio de dados.
     * @param timestamp Timestamp do envio da mensagem.
     */
    void sendData(const char *dataToSend, const char *command, long timestamp);

private:
    /** Servidor WEB. */
    AsyncWebServer *server;
    /** Objeto de eventos WEB. */
    AsyncEventSource *events;
    /** Nome da Rede. */
    const char *ssid;
    /** Senha da Rede. */
    const char *psw;
    /** Último conjunto de dados enviado. */
    const char *lastData;

    /**
     * Inicializa AP.
     */
    void initWiFi();
    
    /**
     * Inicializa SPIFFS.
     */
    void initSPIFFS();
};

#endif
