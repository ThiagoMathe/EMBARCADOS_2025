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
#include "ssd1306.h"
#include "ds18b20.h"
#include "s12.h"

#define WIFI_SSID "Rede"
#define WIFI_PASS "38501236"
#define WEB_SOCKET_PORT 81

#define PINO_UMID 18
#define PINO_TEMP 5
#define OLED_SDA 12
#define OLED_SCL 11
#define LARGURA_OLED 128
#define ALTURA_OLED 64

static const char *TAG = "ESP32-S3";
static ws_client_t ws_client; // Estrutura para o cliente WebSocket

void wifi_init(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

void websocket_callback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len) {
    // Callback para eventos do WebSocket
    switch (type) {
        case WEBSOCKET_CONNECT:
            ESP_LOGI(TAG, "WebSocket conectado!");
            break;
        case WEBSOCKET_DISCONNECT_EXTERNAL:
            ESP_LOGI(TAG, "WebSocket desconectado pelo servidor!");
            break;
        case WEBSOCKET_TEXT:
            ESP_LOGI(TAG, "Mensagem recebida: %.*s", (int)len, msg);
            break;
        default:
            break;
    }
}

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

void sensor_task(void *pvParameters) {
    while (1) {
        // Sensor de umidade
        float umidade = s12_read_analog(PINO_UMID);
        
        // Sensor de temperatura
        float temperatura = ds18b20_read_temp(PINO_TEMP);

        ESP_LOGI(TAG, "Umidade: %.1f%% | Temperatura: %.1fC", umidade, temperatura);
        update_display(temperatura, umidade);

        // Envia os dados via WebSocket
        char json_string[100];
        snprintf(json_string, sizeof(json_string), "{\"temperature\":%.1f, \"humidity\":%.1f}", temperatura, umidade);
        ws_send(&ws_client, WEBSOCKET_OPCODE_TEXT, json_string, strlen(json_string), true);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    // Inicializa o NVS, Wi-Fi e OLED
    nvs_flash_init();
    wifi_init();
    ssd1306_init(OLED_SDA, OLED_SCL);

    // Cria as tarefas
    xTaskCreate(websocket_task, "websocket_task", 4096, NULL, 5, NULL);
    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
}