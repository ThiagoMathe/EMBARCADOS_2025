# Projeto de Desenvolvimento de Bibliotecas para Dispositivos Embarcados

## Visão Geral
Este projeto teve como objetivo o desenvolvimento de bibliotecas para dispositivos embarcados utilizando o microcontrolador ESP32 e o framework ESP-IDF. Além disso, as bibliotecas foram aplicadas em um dispositivo final funcional, demonstrando sua utilidade em um cenário prático.

- ### Link para a documentação detalhada do nosso projeto: [DOCUMENTAÇÃO](https://academico-ifpb-edu-1.gitbook.io/projeto-embarcados)

# Equipe do Projeto:

<div style="display: inline_block;">

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/diegoCBorba" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/90847079?v=4" width="100px;" alt="Avatar Matheus"/><br>
        <sub>
          <b>Diego Cardoso</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/jeremiasmarques" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/90351007?v=4" width="100px;" alt="Avatar Rafael"/><br>
        <sub>
          <b>Jeremias Marques</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/MteusBraga" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/93921419?v=4" width="100px;" alt="Avatar Thiago"/><br>
        <sub>
          <b>Mateus Braga</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/ThiagoMathe" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/89406106?v=4" width="100px;" alt="Avatar Thiago"/><br>
        <sub>
          <b>Thiago Matheus</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

</div>

<details open>
<summary><h2>Etapa 1: Desenvolvimento de Bibliotecas</h2></summary>
Foram desenvolvidas bibliotecas para os seguintes módulos/sensores:

### MPU6050 (I2C)
- **Descrição:** Biblioteca para leitura dos dados do acelerômetro e giroscópio via protocolo I2C.
- **Funcionalidades:**
  - Leitura de aceleração nos eixos X, Y e Z.
  - Leitura de rotação nos eixos X, Y e Z.
  - Leitura da Temperatura em Celsius.
  - Cálculo do Ângulo de Euler.

### SSD1306 (I2C)
- **Descrição:** Biblioteca para controle do display OLED SSD1306 via comunicação I2C.
- **Funcionalidades:**
  - Exibição de texto.
  - Desenho de figuras básicas (pontos, linhas, retângulos, quadrados, círculos e triângulos).
  - Suporte a desenho de texto.

### DS18B20 (1-Wire)
- **Descrição:** Biblioteca para leitura de temperatura utilizando o sensor DS18B20 via comunicação 1-Wire.
- **Funcionalidades:**
  - Leitura de temperatura com resolução configurável.
  - Suporte a múltiplos sensores no mesmo barramento.
  - Conversão automática para graus Celsius.

### SSR (Relé de Estado Sólido)
- **Descrição:** Biblioteca para controle de um Relé de Estado Sólido (SSR) com suporte a acionamento ON-OFF e PWM.
- **Funcionalidades:**
  - Controle ON-OFF do relé.
  - Modulação por Largura de Pulso (PWM) para controle de potência.

### S12 (Sensor de Umidade de Solo)
- **Descrição:** Biblioteca para leitura da umidade do solo utilizando o sensor S12 via comunicação analógica.
- **Funcionalidades:**
  - Leitura do nível de umidade do solo.
  - Cálculo da umidade relativa em percentual.

</details>

---

<details open>
<summary><h2>Etapa 2: Desenvolvimento do Dispositivo Final</h2></summary>

### **Descrição do Projeto**

O dispositivo final desenvolvido foi um **Sistema de Monitoramento e Controle de Ambiente**, integrando os sensores e atuadores implementados nas bibliotecas da Etapa 1. O sistema foi projetado para monitorar temperatura, umidade do solo e ambiente, exibir informações em um display OLED e através de uma aplicação web.

- **Repositório do Projeto Web**: [EMBARCADOS-WEB](https://github.com/diegoCBorba/embarcados-web)

---

### **Funcionalidades Implementadas**

1. **Leitura de Dados dos Sensores**:
   - **Temperatura**: Utilizando o sensor **DS18B20**, o sistema realiza leituras periódicas da temperatura ambiente.
   - **Umidade do Solo**: O sensor **S12** é utilizado para medir a umidade do solo, fornecendo valores analógicos e digitais.

2. **Exibição de Informações no Display OLED**:
   - As informações coletadas pelos sensores são exibidas em tempo real no display **OLED SSD1306**, utilizando a biblioteca desenvolvida para controle do display.

3. **Comunicação via Wi-Fi**:
   - O sistema se conecta a uma rede Wi-Fi para enviar os dados coletados para uma aplicação web.

4. **Monitoramento e Controle Remoto**:
   - Através de uma interface web desenvolvida em **React**, os dados são visualizados em tempo real. A comunicação entre o ESP32 e a aplicação web é feita via **WebSocket**, permitindo atualizações instantâneas.
   -  Bibliotecas WebSocket importadas do [https://github.com/Molorius/esp32-websocket](https://github.com/Molorius/esp32-websocket) 

---

### **Arquitetura do Sistema**

O sistema foi desenvolvido utilizando o **ESP32-S3**, com comunicação entre os módulos via **I2C** e **1-Wire**. A lógica de controle foi implementada em **C utilizando o ESP-IDF**.

#### **Componentes Principais**:
- **Sensores**:
  - **DS18B20**: Sensor de temperatura.
  - **S12**: Sensor de umidade do solo.
- **Display**:
  - **OLED SSD1306**: Exibe as informações coletadas.
- **Comunicação**:
  - **Wi-Fi**: Conecta o ESP32 à rede para comunicação com a aplicação web.
  - **WebSocket**: Protocolo utilizado para enviar dados em tempo real para a aplicação web.

---

### **Implementações Finais**

#### **1. Inicialização do Sistema**
- O sistema inicializa o Wi-Fi, o display OLED e os sensores no `app_main`:
  ```c
  void app_main() {
      // Inicializa o NVS, Wi-Fi e OLED
      nvs_flash_init();
      wifi_init();
      i2c_master_init_ssd();
      ssd1306_init(OLED_SDA, OLED_SCL);

      // Cria as tarefas
      xTaskCreate(websocket_task, "websocket_task", 4096, NULL, 5, NULL);
      xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
  }
  ```

#### **2. Conexão Wi-Fi**
- A função `wifi_init` configura e conecta o ESP32 à rede Wi-Fi:
  ```c
  void wifi_init(void) {
      esp_netif_init();
      esp_event_loop_create_default();
      esp_netif_create_default_wifi_sta();
      wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
      esp_wifi_init(&cfg);
      ESP_LOGI(TAG, "Conectando ao Wi-Fi");
      wifi_config_t wifi_config = {
          .sta = {
              .ssid = WIFI_SSID,
              .password = WIFI_PASS,
          },
      };
      esp_wifi_set_mode(WIFI_MODE_STA);
      esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
      esp_wifi_start();
      ESP_LOGI(TAG, "Wi-Fi conectado!");
  }
  ```

#### **3. Comunicação WebSocket**
- A função `websocket_task` estabelece a conexão com o servidor WebSocket e mantém a comunicação:
  ```c
  void websocket_task(void *pvParameters) {
      // Aguarda a conexão Wi-Fi
      vTaskDelay(pdMS_TO_TICKS(5000));

      // Conecta ao servidor WebSocket
      struct netconn* conn = netconn_new(NETCONN_TCP);
      netconn_connect(conn, IP_ADDR_ANY, WEB_SOCKET_PORT);

      ws_client = ws_connect_client(conn, "/ws", NULL, websocket_callback);

      if (ws_is_connected(ws_client)) {
          ESP_LOGI(TAG, "Cliente WebSocket conectado com sucesso!");
      } else {
          ESP_LOGE(TAG, "Falha ao conectar ao WebSocket!");
          vTaskDelete(NULL);
      }

      while (1) {
          // Mantém a conexão WebSocket ativa
          vTaskDelay(pdMS_TO_TICKS(1000));
      }
  }
  ```

#### **4. Leitura dos Sensores e Atualização do Display**
- A função `sensor_task` realiza a leitura dos sensores e atualiza o display OLED:
  ```c
  void sensor_task(void *pvParameters) {
      while (1) {
          // Sensor de umidade
          float umidade = s12_read_analog(PINO_UMID);
          
          // Sensor de temperatura
          float temperatura = ds18b20_read_temp(PINO_TEMP);

          ESP_LOGI(TAG, "Umidade: %.1f%% Temperatura: %.1fC", umidade, temperatura);
          update_display(temperatura, umidade);

          // Envia os dados via WebSocket
          char json_string[100];
          snprintf(json_string, sizeof(json_string), "{\"temperature\":%.1f, \"humidity\":%.1f}", temperatura, umidade);
          ws_send(&ws_client, WEBSOCKET_OPCODE_TEXT, json_string, strlen(json_string), true);

          ESP_LOGI(TAG, "Enviado: %s", json_string); 

          vTaskDelay(pdMS_TO_TICKS(1000));
      }
  }
  ```

#### **5. Atualização do Display OLED**
- A função `update_display` exibe as informações no display OLED:
  ```c
  void update_display(float temperatura, float umidade) {
      char buffer[16]; // Buffer para armazenar a string formatada

      ssd1306_clear();

      // Formata e desenha a temperatura
      snprintf(buffer, sizeof(buffer), "T: %.1f C", temperatura);
      ssd1306_draw_string(buffer, 0, 10 / 8);

      // Formata e desenha a umidade
      snprintf(buffer, sizeof(buffer), "U: %.1f%%", umidade);
      ssd1306_draw_string(buffer, 0, 40 / 8);

      ssd1306_update_screen();
  }
  ```
</details>
