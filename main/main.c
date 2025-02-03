#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"

static const char *TAG = "SSD1306";

void app_main() {
    i2c_master_init_ssd();
    ssd1306_init();
    ssd1306_clear();

    while (1) {

        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_square(true);
        ssd1306_update_screen();
        ssd1306_draw_square(false);

        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_rectangle(true);
        ssd1306_update_screen();
        ssd1306_draw_rectangle(false);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_triangle(true);
        ssd1306_update_screen();
        ssd1306_draw_triangle(false);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_circle(true);
        ssd1306_update_screen();
        ssd1306_draw_circle(false);

        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_string("ola mundo", 10, 0);


        ESP_LOGI(TAG, "Desenhos feitos com sucesso!");
    }
}