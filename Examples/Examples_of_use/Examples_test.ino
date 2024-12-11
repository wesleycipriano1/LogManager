#include <WiFi.h>
#include <PubSubClient.h>
#include "LogManager.h"

// Defina as variáveis de rede e MQTT
const char* ssid = "sua_rede";
const char* password = "sua_senha";
const char* mqttServer = "broker.mqtt.com";
int mqttPort = 1883;
const char* mqttClientId = "esp32-client";
const char* mqttPrefix = "esp32/"; //prefixo para criação do topico,cada topico é definido por mqttPrefix+quem(nome do dispositivo)

LogManager logManager(ssid, password, mqttServer, mqttPort, mqttClientId, mqttPrefix, 10);

void setup() {
  Serial.begin(115200);
  
  logManager.conectarWiFi();  // Conecta à rede Wi-Fi
  logManager.conectarMQTT();  // Conecta ao broker MQTT
}

void loop() {
  logManager.loop();  // Mantém as conexões ativas
  
  logManager.logDebug("Sensor", "Leitura realizada", "Leitura do sensor foi bem-sucedida", "Usuario1");
  logManager.logWarning("Bateria", "Nível baixo", "Nível de carga abaixo do esperado", "Usuario2");
  logManager.logErro("Sensor", "Falha de leitura", "Leitura falhou", "Usuario3", ERROR_CRITICO);
  
  delay(5000);  // Delay para testar logs periodicamente
}
