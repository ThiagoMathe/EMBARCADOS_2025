/*
#include "ssr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void app_main(void) {
    ssr_t ssr = {
        .gpio = GPIO_NUM_9,
        .mode = SSR_MODE_PWM,  // Alternar para SSR_MODE_ON_OFF se necessário
        .pwm_channel = LEDC_CHANNEL_0,
        .pwm_freq = 1000 // Frequência PWM de 1 kHz
    };

    ssr_init(&ssr);

    while (1) {
        ssr_set_state(&ssr, 1);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ssr_set_state(&ssr, 0);
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Testando PWM (se o modo estiver configurado para PWM)
        for (int i = 0; i <= 100; i += 10) {
            ssr_set_duty(&ssr, i);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

*/

#include <stdio.h>
#include "ds18b20.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define DS18B20_GPIO 18
static const char *TAG = "DS18B20";

void app_main() {
    ds18b20_init(DS18B20_GPIO);

    uint8_t sensor1[8] = {0x28, 0xD6, 0xE0, 0x03, 0x00, 0x00, 0x80, 0xB6};
    uint8_t sensor2[8] = {0x28, 0xE7, 0x2C, 0x57, 0x04, 0xEB, 0x3C, 0xE0};

    while (1) {
        float temperatura1 = ds18b20_read_temp_address(DS18B20_GPIO, sensor1);
        float temperatura2 = ds18b20_read_temp_address(DS18B20_GPIO, sensor2);

        if (temperatura1 > -1000) {
            ESP_LOGI(TAG, "Temperatura do Sensor 1: %.2f °C", temperatura1);
        }
        if (temperatura2 > -1000) {
            ESP_LOGI(TAG, "Temperatura do Sensor 2: %.2f °C", temperatura2);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
