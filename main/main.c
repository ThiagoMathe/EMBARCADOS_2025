#include <stdio.h>
#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    i2c_master_init();
    mpu6050_init();

    while (1) {
        mpu6050_read_data();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Atualiza os dados a cada 1 segundo
    }
}