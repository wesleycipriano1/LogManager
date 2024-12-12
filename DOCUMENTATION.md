# Documentação da Biblioteca LogManager

## Visão Geral

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
```
## Classes e Métodos
Classe LogManager
Construtor:

```cpp
LogManager(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttClientId, const char* mqttPrefix, size_t bufferSize, const char* ntpServer = "pool.ntp.org", long timeOffset = -3 * 3600);
```
## Parâmetros:

ssid: Nome da rede Wi-Fi.

password: Senha da rede Wi-Fi.

mqttServer: Endereço do servidor MQTT.

mqttPort: Porta do servidor MQTT.

mqttClientId: ID do cliente MQTT.

mqttPrefix: Prefixo usado nos tópicos MQTT.

bufferSize: Tamanho do buffer de logs.

ntpServer: Servidor NTP para sincronização de tempo.

timeOffset: Deslocamento de tempo para ajuste do fuso horário.

## Destrutor:

```cpp
~LogManager();
```
Descrição: Libera os recursos alocados pela classe.

## Métodos Públicos:

void conectarWiFi();: Conecta o dispositivo à rede Wi-Fi especificada.

void conectarMQTT();: Conecta o dispositivo ao broker MQTT.

void loop();: Mantém as conexões Wi-Fi e MQTT.

void logDebug(const String& onde, const String& como, const String& porQue, const String& quem);: Envia um log de depuração.

void logWarning(const String& onde, const String& como, const String& porQue, const String& quem);: Envia um log de aviso.

void logErro(const String& onde, const String& como, const String& porQue, const String& quem, ErrorType errorType);: Envia um log de erro, podendo ser crítico ou não crítico.

## Privados:

String obterHoraAtual();: Obtém a hora atual utilizando o servidor NTP.

void enviarLogDireto(const String& tipo, const String& onde, const String& como, const String& porQue, const String& quem);: Envia um log diretamente via MQTT.

void enviarLogBuffer();: Envia logs armazenados no buffer via MQTT.

### Estrutura Log
```cpp
struct Log {
    String tipo;   // Tipo do log (debug, warning, error)
    String onde;   // Local onde ocorreu o evento
    String como;   // Descrição de como ocorreu o evento
    String quando; // Momento em que o evento ocorreu
    String porQue; // Motivo do evento
    String quem;   // Identificação de quem gerou o log
    bool status;   // Status do log (enviado ou não)
};
Enums
ErrorType:

cpp
enum ErrorType {
    ERROR_CRITICO,   // Indica um erro crítico que deve ser enviado imediatamente
    ERROR_NAO_CRITICO // Indica um erro não crítico que pode ser armazenado no buffer
};
``` 
### Dependências
WiFi: Biblioteca padrão do Arduino para conexão Wi-Fi.

PubSubClient: Biblioteca para conexão com o broker MQTT.

ArduinoJson: Biblioteca para manipulação de JSON.

NTPClient: Biblioteca para sincronização de tempo via NTP.

TimeLib: Biblioteca para manipulação de tempo.

## Licença
Este projeto está licenciado sob os termos da licença MIT. Consulte o arquivo LICENSE para mais informações.