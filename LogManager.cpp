#include "LogManager.h"

// Construtor da classe LogManager
LogManager::LogManager(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttClientId,
                       const char* mqttPrefix, size_t bufferSize, const char* ntpServer, long timeOffset)
    : ssid(ssid), password(password), mqttServer(mqttServer), mqttPort(mqttPort), mqttClientId(mqttClientId),
      mqttPrefix(mqttPrefix), bufferSize(bufferSize), bufferIndex(0), client(espClient), timeClient(udp, ntpServer, timeOffset, 3600 * 24) {
    buffer = new Log[bufferSize];
}

LogManager::~LogManager() {
    delete[] buffer;
}

// Método para manter a conexão Wi-Fi
void LogManager::conectarWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado ao Wi-Fi!");
}

// Método para manter a conexão MQTT
void LogManager::conectarMQTT() {
    client.setServer(mqttServer, mqttPort);
    while (!client.connected()) {
        Serial.println("Conectando ao broker MQTT...");
        if (client.connect(mqttClientId)) {
            Serial.println("Conectado ao broker MQTT!");
        } else {
            Serial.print("Falha na conexão MQTT. Código: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

// Método que mantém a conexão Wi-Fi e MQTT automaticamente
void LogManager::loop() {
    if (WiFi.status() != WL_CONNECTED) {
        conectarWiFi(); // Tenta reconectar ao Wi-Fi se necessário
    }

    if (!client.connected()) {
        conectarMQTT(); // Tenta reconectar ao MQTT se necessário
    }

    client.loop(); // Mantém a comunicação MQTT ativa
}

String LogManager::obterHoraAtual() {
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    String formattedTime = String(hour(epochTime)) + ":" + String(minute(epochTime)) + ":" + String(second(epochTime)) + " " +
                           String(day(epochTime)) + "/" + String(month(epochTime)) + "/" + String(year(epochTime));
    return formattedTime;
}

void LogManager::enviarLogDireto(const String& tipo, const String& onde, const String& como, const String& porQue, const String& quem) {
    if (WiFi.status() == WL_CONNECTED && client.connected()) {
        String mqttTopic = String(mqttPrefix) + quem;

        StaticJsonDocument<200> jsonDoc;
        jsonDoc["tipo"] = tipo;
        jsonDoc["onde"] = onde;
        jsonDoc["como"] = como;
        jsonDoc["quando"] = obterHoraAtual();
        jsonDoc["porQue"] = porQue;
        jsonDoc["quem"] = quem;
        jsonDoc["status"] = false;

        String jsonString;
        serializeJson(jsonDoc, jsonString);

        if (client.publish(mqttTopic.c_str(), jsonString.c_str())) {
            Serial.println("Log enviado: " + jsonString);
        } else {
            Serial.println("Erro ao enviar log crítico MQTT");
        }
    } else {
        Serial.println("Erro: Wi-Fi ou MQTT desconectado");
    }
}

void LogManager::enviarLogBuffer() {
    if (WiFi.status() == WL_CONNECTED && client.connected()) {
        for (size_t i = 0; i < bufferIndex; i++) {
            String mqttTopic = String(mqttPrefix) + buffer[i].quem;

            StaticJsonDocument<200> jsonDoc;
            jsonDoc["tipo"] = buffer[i].tipo;
            jsonDoc["onde"] = buffer[i].onde;
            jsonDoc["como"] = buffer[i].como;
            jsonDoc["quando"] = buffer[i].quando;
            jsonDoc["porQue"] = buffer[i].porQue;
            jsonDoc["quem"] = buffer[i].quem;
            jsonDoc["status"] = buffer[i].status;

            String jsonString;
            serializeJson(jsonDoc, jsonString);

            if (client.publish(mqttTopic.c_str(), jsonString.c_str())) {
                Serial.println("Log enviado: " + jsonString);
            } else {
                Serial.println("Erro ao enviar mensagem MQTT");
            }
        }
        bufferIndex = 0;
    } else {
        Serial.println("Erro: Wi-Fi ou MQTT desconectado");
    }
}

void LogManager::logDebug(const String& onde, const String& como, const String& porQue, const String& quem) {
    if (bufferIndex < bufferSize) {
        buffer[bufferIndex++] = { "debug", onde, como, obterHoraAtual(), porQue, quem, false };
    }
    if (bufferIndex == bufferSize) {
        enviarLogBuffer();
    }
}

void LogManager::logWarning(const String& onde, const String& como, const String& porQue, const String& quem) {
    if (bufferIndex < bufferSize) {
        buffer[bufferIndex++] = { "warning", onde, como, obterHoraAtual(), porQue, quem, false };
    }
    if (bufferIndex == bufferSize) {
        enviarLogBuffer();
    }
}

void LogManager::logErro(const String& onde, const String& como, const String& porQue, const String& quem, ErrorType errorType) {
    if (errorType == ERROR_CRITICO) {
        enviarLogDireto("error", onde, como, porQue, quem);
    } else {
        if (bufferIndex < bufferSize) {
            buffer[bufferIndex++] = { "error", onde, como, obterHoraAtual(), porQue, quem, false };
        }
        if (bufferIndex == bufferSize) {
            enviarLogBuffer();
        }
    }


}
