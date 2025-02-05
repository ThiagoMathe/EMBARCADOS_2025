#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"

static const char *TAG = "SSD1306";

void app_main() {
    i2c_master_init_ssd();
    ssd1306_init();
    ssd1306_clear();
    ESP_LOGI(TAG, "Inicio com sucesso!");

    while (1) {

        vTaskDelay(pdMS_TO_TICKS(1000));
        ssd1306_draw_square(true);
        ssd1306_update_screen();
        ssd1306_draw_square(false);

        ESP_LOGI(TAG, "Quadrado com sucesso!");

        vTaskDelay(pdMS_TO_TICKS(1000));
        ssd1306_draw_rectangle(true);
        ssd1306_update_screen();
        ssd1306_draw_rectangle(false);

        ESP_LOGI(TAG, "Retangulo com sucesso!");
        
        vTaskDelay(pdMS_TO_TICKS(1000));
        ssd1306_draw_triangle(true);
        ssd1306_update_screen();
        ssd1306_draw_triangle(false);

        ESP_LOGI(TAG, "Triangulo com sucesso!");
        
        vTaskDelay(pdMS_TO_TICKS(1000));
        ssd1306_draw_circle(true);
        ssd1306_update_screen();
        ssd1306_draw_circle(false);

        ESP_LOGI(TAG, "Circulo com sucesso!");

        vTaskDelay(pdMS_TO_TICKS(1000));
        ssd1306_update_screen();
        ssd1306_draw_string("ola mundo", 10, 0);


        ESP_LOGI(TAG, "Desenhos feitos com sucesso!");
    }
}