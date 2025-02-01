#include <stdio.h>
#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    i2c_master_init();
    mpu6050_init();

while (1) {
        // Ler temperatura
        float temperature = read_temperature();
        if (temperature != -1.0 && temperature != 36.53) {
            printf("Temperatura: %.2f°C\n", temperature);
        }

        // Ler aceleração
        float accel_x, accel_y, accel_z;
        read_acceleration(&accel_x, &accel_y, &accel_z);

        if(!(accel_x == accel_y && accel_z == 0 && accel_x == 0)) {
            printf("Aceleração: X=%.2fg, Y=%.2fg, Z=%.2fg\n", accel_x, accel_y, accel_z);
        }

        // Ler giroscópio
        float gyro_x, gyro_y, gyro_z;
        read_gyroscope(&gyro_x, &gyro_y, &gyro_z);

        if(!(gyro_x == gyro_y && gyro_z == 0 && gyro_x == 0)) {
            printf("Giroscópio: X=%.2f°/s, Y=%.2f°/s, Z=%.2f°/s\n", gyro_x, gyro_y, gyro_z);
        }


        // Angulo de Euler
        float roll, pitch, yaw;
        float dt = 1.0 / 100.0;
        calculate_euler_angles(&roll, &pitch, &yaw, dt);

        if(!(roll == 0 && pitch == 0 && yaw == 0)) {
            printf("Euler: roll=%.2f, pitch=%.2f, yaw=%.2f\n", roll, pitch, yaw);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Atualiza os dados a cada 1 segundo
    }
}