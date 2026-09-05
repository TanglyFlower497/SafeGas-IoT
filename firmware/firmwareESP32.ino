#include <WiFi.h>
#include <PubSubClient.h>

// ===================== CONFIGURAÇÕES =====================
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port = 1883;
const char* mqtt_topic = "ifsuldeminas/iot/grupo05/gas";

#define MQ2_PIN 34          // GPIO34 = ADC1, pino somente entrada (correto para sensores analógicos)
#define NUM_AMOSTRAS 10     // quantidade de leituras para a média (reduz ruído do ADC)
#define INTERVALO_PUBLICACAO 2000UL   // ms entre publicações
#define TEMPO_AQUECIMENTO 20000UL     // ms de preheat do MQ2 (mínimo recomendado; ideal: mais tempo)
#define WIFI_TIMEOUT_TENTATIVAS 40    // ~20s de tentativa (40 * 500ms) antes de desistir e tentar de novo

WiFiClient espClient;
PubSubClient client(espClient);

// ===================== WIFI =====================
bool setup_wifi() {
  WiFi.mode(WIFI_STA);   // evita ambiguidade entre modos STA/AP
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < WIFI_TIMEOUT_TENTATIVAS) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado! IP: " + WiFi.localIP().toString());
    return true;
  } else {
    Serial.println("\nFalha ao conectar ao WiFi (timeout).");
    return false;
  }
}

// ===================== MQTT =====================
void reconnect() {
  int tentativas = 0;
  while (!client.connected() && tentativas < 5) {
    // client id único: agora com seed real, evita colisão entre placas do grupo
    String clientId = "ESP32GasClient-" + String(random(0xffff), HEX);
    Serial.print("Tentando conexão MQTT... ");

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado como " + clientId);
      return;
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2s");
      delay(2000);
      tentativas++;
    }
  }
}

// ===================== LEITURA DO SENSOR =====================
// Faz várias leituras e retorna a média, reduzindo o ruído típico do ADC do ESP32
int lerGasMedia() {
  long soma = 0;
  for (int i = 0; i < NUM_AMOSTRAS; i++) {
    soma += analogRead(MQ2_PIN);
    delay(10);
  }
  return soma / NUM_AMOSTRAS;
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);
  pinMode(MQ2_PIN, INPUT);

  // Semente real para random() -> evita client IDs repetidos entre boots/placas
  randomSeed(esp_random());

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  // Tempo de aquecimento do MQ2: o sensor precisa estabilizar antes de dar leituras confiáveis.
  // NOTA: 20s é o mínimo aceitável para fins didáticos; em uso real, o datasheet recomenda
  // um pré-aquecimento bem mais longo (vários minutos) para maior precisão.
  Serial.println("Aquecendo sensor MQ2, aguarde...");
  delay(TEMPO_AQUECIMENTO);
  Serial.println("Sensor pronto.");
}

// ===================== LOOP =====================
void loop() {
  // Garante que o WiFi esteja de pé antes de mexer no MQTT
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi caiu, tentando reconectar...");
    setup_wifi();
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > INTERVALO_PUBLICACAO) {
    lastMsg = millis();

    int valorGas = lerGasMedia();

    // Payload em JSON simples: mais fácil de consumir do lado do dashboard/servidor
    // OBS: valorGas ainda é a leitura bruta do ADC (0-4095), não uma concentração em ppm.
    // Para obter ppm real seria necessário calibrar o R0 do MQ2 e aplicar a curva do datasheet.
    char msg[64];
    snprintf(msg, sizeof(msg), "{\"gas\":%d,\"ts\":%lu}", valorGas, millis());

    Serial.print("Leitura Gás ESP32 (GPIO34): ");
    Serial.println(msg);

    if (client.publish(mqtt_topic, msg)) {
      Serial.println("Publicado com sucesso.");
    } else {
      Serial.println("Falha ao publicar no MQTT.");
    }
  }
}
