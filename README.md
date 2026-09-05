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
* **Tópico MQTT:** `ifsuldeminas/iot/grupo05/gas`[cite: 1]

---

## 🏗️ Arquitetura da Solução
---

## 🔌 Ligação do Hardware (ESP32)

| Pino do Sensor MQ-2 | Pino do ESP32 | Função |
| :--- | :--- | :--- |
| **VCC** | **5V / VIN** | Alimentação do filamento do sensor |
| **GND** | **GND** | Aterramento de referência |
| **AO** *(Analog Out)* | **GPIO 34** | Leitura analógica (Conversor ADC1) |




## 🔌 Ligação do Hardware (ESP8266)

| Pino do MQ2 | Vai para (NodeMCU/Wemos) | Observação |
|---|---|---|
| **VCC** | **VIN** (NodeMCU) ou **5V** (Wemos D1 Mini) | **Não use o 3V3.** O aquecedor do MQ2 precisa de ~5V estáveis; essas placas repassam o 5V do USB por esse pino. |
| **GND** | Qualquer **GND** da placa | Tem mais de um GND disponível na maioria das placas — use qualquer um. |
| **AO** (saída analógica) | **A0**, passando antes por um divisor resistivo | A0 é o único pino analógico do ESP8266. Como o AO do MQ2 pode chegar perto dos 5V do Vcc, e o A0 do ESP8266 só é seguro até ~3.3V, intercale o divisor (ex.: 1kΩ em série + 2kΩ para o GND) entre o AO do sensor e o A0 da placa. |
| **DO** (saída digital) | **Não conecte** (ou GPIO livre, se quiser usar depois) | O firmware atual só lê a saída analógica — o `DO` fica sem função no código de vocês. Se quiser deixar disponível para um uso futuro, ligue a qualquer GPIO livre como **D5**, **D6** ou **D7**. |

Duas observações importantes:

1. **Cuidado com o nome "D0"**: o pino de saída digital do **módulo MQ2** se chama D0/DO, mas a própria placa NodeMCU **também tem um pino chamado "D0"** (que corresponde ao GPIO16) — são coisas diferentes com o mesmo nome. Por isso evitei sugerir o D0 da placa para não gerar confusão; prefira D5/D6/D7 se for ligar o DO do sensor.
2. Como o `DO` não é usado pelo código, a forma mais simples é **simplesmente deixá-lo desconectado** — três fios (VCC, GND, AO) já são suficientes para o projeto funcionar como está.

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
### Simulação no Wokwi(ESP32)
1. https://wokwi.com/projects/474283759267430401
