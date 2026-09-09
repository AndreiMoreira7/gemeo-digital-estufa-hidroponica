// Enviando dados do sensor de temperatura para o things board a fins de teste;
// Usando arduino uno e ethernet shield.

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configurações do Sensor DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Endereço MAC único para o Ethernet Shield
byte mac[] = { 0x70, 0xB3, 0xD5, 0x0A, 0xC6, 0xFF };

// Servidor ThingsBoard Cloud (Europa) e Porta MQTT
const char* tbServer = "eu.thingsboard.cloud";
const int tbPort = 1883;

const char* TOKEN = "Uxezf4pVy7erzsMEVANH";

EthernetClient ethClient;
PubSubClient client(ethClient);

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println(F("Iniciando conexao Ethernet..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Falha ao obter IP via DHCP"));
    for (;;);
  }
  
  Serial.print(F("IP Obtido: "));
  Serial.println(Ethernet.localIP());

  client.setServer(tbServer, tbPort);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print(F("Conectando ao ThingsBoard... "));
    // O Access Token é passado como Username no protocolo MQTT do ThingsBoard
    if (client.connect("ArduinoUnoClient", TOKEN, NULL)) {
      Serial.println(F("[OK]"));
    } else {
      Serial.print(F("[FALHA] Rc="));
      Serial.print(client.state());
      Serial.println(F(" Tentando novamente em 5s..."));
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Envia telemetria a cada 10 segundos
  if (millis() - lastSend > 10000) {
    lastSend = millis();

    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(umidade) || isnan(temperatura)) {
      Serial.println(F("Falha ao ler o sensor DHT11!"));
      return;
    }

    // Monta o JSON de telemetria
    String payload = "{\"temperature\":";
    payload += temperatura;
    payload += ",\"humidity\":";
    payload += umidade;
    payload += "}";

    Serial.print(F("Enviando dados: "));
    Serial.println(payload);

    // Publica no tópico padrão de telemetria do ThingsBoard
    client.publish("v1/devices/me/telemetry", payload.c_str());
  }
}