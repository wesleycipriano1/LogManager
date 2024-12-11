markdown
# LogManager Library

A biblioteca **LogManager** permite a conexão Wi-Fi e MQTT para o envio de logs de eventos em dispositivos IoT. Além disso, armazena logs em um buffer para serem enviados posteriormente, caso a conexão esteja indisponível.

## Funcionalidades

- **Conexão Wi-Fi:** Conecta o dispositivo a uma rede Wi-Fi especificada.
- **Conexão MQTT:** Conecta o dispositivo a um broker MQTT para envio de logs.
- **Envio de Logs:** Envia logs diretamente ou armazena-os em um buffer para envio posterior.
- **Sincronização NTP:** Obtém a hora atual utilizando um servidor NTP.

## Como Utilizar

### Instalação

1. Clone o repositório ou faça o download dos arquivos.
2. Inclua os arquivos da biblioteca no seu projeto Arduino.

### Exemplos de Uso

```cpp
#include "LogManager.h"

// Configurações de rede e MQTT
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* mqttServer = "YOUR_MQTT_SERVER";
int mqttPort = 1883;
const char* mqttClientId = "YOUR_CLIENT_ID";
const char* mqttPrefix = "logs/";
const char* ntpServer = "pool.ntp.org";
long timeOffset = -3 * 3600; // Ajuste para seu fuso horário

// Criação do objeto LogManager
LogManager logManager(ssid, password, mqttServer, mqttPort, mqttClientId, mqttPrefix, 10, ntpServer, timeOffset);

void setup() {
    Serial.begin(115200);
    logManager.conectarWiFi();
    logManager.conectarMQTT();
}

void loop() {
    logManager.loop();

    // Enviar logs de exemplo
    logManager.logDebug("MainLoop", "Início do loop", "Verificação de estado", "Sistema");
    delay(10000); // Atraso para simular tempo entre eventos
}
Dependências
WiFi: Biblioteca padrão do Arduino para conexão Wi-Fi.

PubSubClient: Biblioteca para conexão com o broker MQTT.

ArduinoJson: Biblioteca para manipulação de JSON.

NTPClient: Biblioteca para sincronização de tempo via NTP.

TimeLib: Biblioteca para manipulação de tempo.

Licença
Este projeto está licenciado sob os termos da licença MIT. Consulte o arquivo LICENSE para mais informações.