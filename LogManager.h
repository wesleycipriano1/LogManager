#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

enum ErrorType {
    ERROR_CRITICO,
    ERROR_NAO_CRITICO
};

struct Log {
    String tipo;
    String onde;
    String como;
    String quando;
    String porQue;
    String quem;
    bool status;
};

class LogManager {
public:
    LogManager(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttClientId,
               const char* mqttPrefix, size_t bufferSize, const char* ntpServer = "pool.ntp.org", long timeOffset = -3 * 3600);
    ~LogManager();

    // Métodos públicos
    void conectarWiFi();
    void conectarMQTT();
    
    void logDebug(const String& onde, const String& como, const String& porQue, const String& quem);
    void logWarning(const String& onde, const String& como, const String& porQue, const String& quem);
    void logErro(const String& onde, const String& como, const String& porQue, const String& quem, ErrorType errorType);


    // Método que mantém a conexão automaticamente
    void loop();

private:
    const char* ssid;
    const char* password;
    const char* mqttServer;
    int mqttPort;
    const char* mqttClientId;
    const char* mqttPrefix;
    size_t bufferSize;
    size_t bufferIndex;
    Log* buffer;

    WiFiClient espClient;
    PubSubClient client;
    WiFiUDP udp;
    NTPClient timeClient;

    String obterHoraAtual();
    void enviarLogDireto(const String& tipo, const String& onde, const String& como, const String& porQue, const String& quem);
    void enviarLogBuffer();
};

#endif
