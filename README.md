# SafeGas IoT — GasGuard Pro 

Projeto IoT para detecção contínua de vazamentos de gás inflamável, envio de dados via protocolo MQTT e monitoramento em tempo real através de uma dashboard web. Desenvolvido para a disciplina de Robótica e IoT do IFSULDEMINAS - Campus Machado[cite: 1].

---

## 🏢 Definição do Produto & Empresa
* **Empresa:** SafeGas IoT[cite: 1]
* **Produto:** GasGuard Pro[cite: 1]
* **Problema Resolvido:** Prevenção de acidentes e explosões residenciais/comerciais através da detecção precoce de vazamentos de gás[cite: 1].
* **Sensor:** MQ-2 / MQ-5 (Gás / Fumaça)[cite: 1]
* **Métrica / Unidade:** Concentração em PPM (Partes Por Milhão)[cite: 1]
* **Broker MQTT:** `broker.hivemq.com` (Porta WebSockets: `8884` / TCP: `1883`)[cite: 1]
* **Tópico MQTT:** `ifsuldeminas/iot/grupo01/gas`[cite: 1]

---

## 🏗️ Arquitetura da Solução
---

## 🔌 Ligação do Hardware (ESP32 + MQ-2)

| Pino do Sensor MQ-2 | Pino do ESP32 | Função |
| :--- | :--- | :--- |
| **VCC** | **5V / VIN** | Alimentação do filamento do sensor |
| **GND** | **GND** | Aterramento de referência |
| **AO** *(Analog Out)* | **GPIO 34** | Leitura analógica (Conversor ADC1) |

---

## 🛠️ Como Executar o Projeto

### 1. Firmware (ESP32)
1. Instale a biblioteca **PubSubClient** na Arduino IDE[cite: 1].
2. Abra o arquivo `firmware/firmware.ino`.
3. Atualize as constantes `ssid` e `password` com os dados da sua rede Wi-Fi[cite: 1].
4. Selecione a placa **ESP32 Dev Module** e faça o upload.

### 2. Dashboard Web
1. Abra a pasta `dashboard/` e dê duplo clique no arquivo `index.html`.
2. A página conectará automaticamente ao broker via WebSockets[cite: 1].
3. O status mudará para **Conectado** e os valores em PPM serão atualizados a cada envio do ESP32[cite: 1].
