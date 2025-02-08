#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ds18b20.h"

void app_main() {
    while (1) {
        float temperature = ds18b20_get_temperature();
        printf("Temperatura obtida: %.2f°C\n", temperature);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
