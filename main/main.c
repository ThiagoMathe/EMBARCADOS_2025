#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/sys.h"
#include "driver/i2c.h"

// Bibliotecas WebSocket importadas do https://github.com/Molorius/esp32-websocket
#include "websocket.h"
#include "websocket_server.h"

// Nossas Bibliotecas
#include "ssd1306.h"   // Biblioteca para controle do display OLED
#include "ds18b20.h"   // Biblioteca para leitura do sensor de temperatura DS18B20
#include "s12.h"       // Biblioteca para leitura do sensor de umidade do solo S12

// Definições de configuração
#define WIFI_SSID "Rede"       // Nome da rede Wi-Fi
#define WIFI_PASS "38501236"   // Senha da rede Wi-Fi
#define WEB_SOCKET_PORT 81     // Porta do servidor WebSocket

// Definições dos pinos
#define PINO_UMID 18           // Pino do sensor de umidade do solo (analógico)
#define PINO_TEMP 5            // Pino do sensor de temperatura DS18B20
#define OLED_SDA 12            // Pino SDA do display OLED
#define OLED_SCL 11            // Pino SCL do display OLED
#define LARGURA_OLED 128       // Largura do display OLED
#define ALTURA_OLED 64         // Altura do display OLED

static const char *TAG = "ESP32-S3"; // Tag para logs
static ws_client_t ws_client;        // Estrutura para o cliente WebSocket

// Função para inicializar o Wi-Fi
void wifi_init(void) {
    esp_netif_init();                     // Inicializa a pilha de rede
    esp_event_loop_create_default();      // Cria o loop de eventos padrão
    esp_netif_create_default_wifi_sta();  // Cria a interface Wi-Fi em modo station
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // Configuração padrão do Wi-Fi
    esp_wifi_init(&cfg);                  // Inicializa o Wi-Fi com a configuração
    ESP_LOGI(TAG, "Conectando ao Wi-Fi");

    // Configuração da rede Wi-Fi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,      // Nome da rede Wi-Fi
            .password = WIFI_PASS,  // Senha da rede Wi-Fi
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);               // Define o modo Wi-Fi como station
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config); // Aplica a configuração
    esp_wifi_start();                               // Inicia o Wi-Fi
    ESP_LOGI(TAG, "Wi-Fi conectado!");
}

// Callback para eventos do WebSocket
void websocket_callback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len) {
    switch (type) {
        case WEBSOCKET_CONNECT:
            ESP_LOGI(TAG, "WebSocket conectado"); // Log quando o WebSocket é conectado
            break;
        case WEBSOCKET_DISCONNECT_EXTERNAL:
            ESP_LOGI(TAG, "WebSocket desconectado"); // Log quando o WebSocket é desconectado
            break;
        case WEBSOCKET_TEXT:
            ESP_LOGI(TAG, "Mensagem recebida: %.*s", (int)len, msg); // Log de mensagens recebidas
            break;
        default:
            break;
    }
}

// Tarefa para gerenciar a conexão WebSocket
void websocket_task(void *pvParameters) {
    // Aguarda a conexão Wi-Fi
    vTaskDelay(pdMS_TO_TICKS(5000));

    // Conecta ao servidor WebSocket
    struct netconn* conn = netconn_new(NETCONN_TCP); // Cria uma nova conexão TCP
    netconn_connect(conn, IP_ADDR_ANY, WEB_SOCKET_PORT); // Conecta ao servidor WebSocket

    // Inicia a conexão WebSocket
    ws_client = ws_connect_client(conn, "/ws", NULL, websocket_callback);

    // Verifica se a conexão foi bem-sucedida
    if (ws_is_connected(ws_client)) {
        ESP_LOGI(TAG, "Cliente WebSocket conectado com sucesso!");
    } else {
        ESP_LOGE(TAG, "Falha ao conectar ao WebSocket!");
        vTaskDelete(NULL); // Encerra a tarefa em caso de falha
    }

    // Loop para manter a conexão WebSocket ativa
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo
    }
}

// Função para atualizar o display OLED com os dados dos sensores
void update_display(float temperatura, float umidade) {
    char buffer[16]; // Buffer para armazenar a string formatada

    ssd1306_clear(); // Limpa o display

    // Formata e desenha a temperatura
    snprintf(buffer, sizeof(buffer), "T: %.1f C", temperatura);
    ssd1306_draw_string(buffer, 0, 10 / 8); // Desenha no display (linha 1)

    // Formata e desenha a umidade
    snprintf(buffer, sizeof(buffer), "U: %.1f%%", umidade);
    ssd1306_draw_string(buffer, 0, 40 / 8); // Desenha no display (linha 2)

    ssd1306_update_screen(); // Atualiza o display
}

// Tarefa para leitura dos sensores e envio dos dados via WebSocket
void sensor_task(void *pvParameters) {
    while (1) {
        // Leitura do sensor de umidade do solo
        float umidade = s12_read_analog(PINO_UMID);
        
        // Leitura do sensor de temperatura
        float temperatura = ds18b20_read_temp(PINO_TEMP);

        // Log dos valores lidos
        ESP_LOGI(TAG, "Umidade: %.1f%% Temperatura: %.1fC", umidade, temperatura);

        // Atualiza o display OLED com os valores lidos
        update_display(temperatura, umidade);

        // Formata os dados em JSON para envio via WebSocket
        char json_string[100];
        snprintf(json_string, sizeof(json_string), "{\"temperature\":%.1f, \"humidity\":%.1f}", temperatura, umidade);

        // Envia os dados via WebSocket
        ws_send(&ws_client, WEBSOCKET_OPCODE_TEXT, json_string, strlen(json_string), true);

        // Log da mensagem enviada
        ESP_LOGI(TAG, "Enviado: %s", json_string); 

        // Aguarda 1 segundo antes da próxima leitura
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Função principal do programa
void app_main() {
    // Inicializa o NVS (Non-Volatile Storage)
    nvs_flash_init();

    // Inicializa o Wi-Fi
    wifi_init();

    // Inicializa o barramento I2C e o display OLED
    i2c_master_init_ssd();
    ssd1306_init(OLED_SDA, OLED_SCL);

    // Cria as tarefas para WebSocket e leitura dos sensores
    xTaskCreate(websocket_task, "websocket_task", 4096, NULL, 5, NULL);
    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
}