#include <stdio.h>
#include "mpu6050.h"

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_NUM I2C_NUM_0        // Número do I2C (I2C0)
#define I2C_MASTER_SDA_IO 21           // GPIO para SDA
#define I2C_MASTER_SCL_IO 20           // GPIO para SCL
#define I2C_MASTER_FREQ_HZ 100000      // Frequência do barramento I2C
#define MPU6050_ADDR 0x68              // Endereço do MPU6050
#define MPU6050_WHO_AM_I 0x75          // Registrador de identificação do MPU6050
#define MPU6050_PWR_MGMT_1 0x6B        // Registrador de gerenciamento de energia
#define MPU6050_ACCEL_XOUT_H 0x3B      // Registrador de saída do eixo X do acelerômetro

static const char *TAG = "MPU6050";
void mpu6050_init() {
    uint8_t data[2];

    // Desativa o modo de sleep
    data[0] = MPU6050_PWR_MGMT_1;  // Endereço do registrador
    data[1] = 0x00;                // Valor para desativar o modo de sleep
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, MPU6050_ADDR, data, 2, pdMS_TO_TICKS(100)));

    ESP_LOGI(TAG, "MPU6050 inicializado");
}

esp_err_t read_mpu6050(uint8_t reg_addr, uint8_t *data, size_t len) {
    // Escreve o endereço do registrador que será lido
    esp_err_t ret = i2c_master_write_read_device(
        I2C_MASTER_NUM,
        MPU6050_ADDR,
        &reg_addr,
        1,
        data,
        len,
        pdMS_TO_TICKS(100)
    );
    return ret;
}

void get_accel_data() {
    uint8_t data[6]; // 6 bytes: 2 para cada eixo (X, Y, Z)
    esp_err_t ret = read_mpu6050(MPU6050_ACCEL_XOUT_H, data, 6);

    if (ret == ESP_OK) {
        int16_t accel_x = (int16_t)(data[0] << 8 | data[1]);
        int16_t accel_y = (int16_t)(data[2] << 8 | data[3]);
        int16_t accel_z = (int16_t)(data[4] << 8 | data[5]);

        ESP_LOGI(TAG, "Accel X: %d, Accel Y: %d, Accel Z: %d", accel_x, accel_y, accel_z);
    } else {
        ESP_LOGE(TAG, "Erro ao ler dados do MPU6050");
    }
}

void i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
}
void mpu6050_read_data() {
    uint8_t data[14]; // 6 bytes para acelerômetro, 2 para temperatura, 6 para giroscópio
    esp_err_t ret = i2c_master_write_read_device(
        I2C_MASTER_NUM,
        MPU6050_ADDR,
        (uint8_t[]){MPU6050_ACCEL_XOUT_H},
        1,
        data,
        14,
        pdMS_TO_TICKS(100)
    );

    if (ret == ESP_OK) {
        // Converter os valores lidos (big-endian para little-endian)
        int16_t accel_x = (int16_t)(data[0] << 8 | data[1]);
        int16_t accel_y = (int16_t)(data[2] << 8 | data[3]);
        int16_t accel_z = (int16_t)(data[4] << 8 | data[5]);

        int16_t temp_raw = (int16_t)(data[6] << 8 | data[7]);
        float temperature = (temp_raw / 340.0) + 36.53; // Fórmula do datasheet

        int16_t gyro_x = (int16_t)(data[8] << 8 | data[9]);
        int16_t gyro_y = (int16_t)(data[10] << 8 | data[11]);
        int16_t gyro_z = (int16_t)(data[12] << 8 | data[13]);

        // Exibir os valores no terminal
        printf("Acelerômetro: X=%d, Y=%d, Z=%d\n", accel_x, accel_y, accel_z);
        printf("Temperatura: %.2f°C\n", temperature);
        printf("Giroscópio: X=%d, Y=%d, Z=%d\n", gyro_x, gyro_y, gyro_z);
    } else {
        printf("Erro ao ler dados do sensor MPU6050\n");
    }
}


void app_main() {
    i2c_master_init();
    mpu6050_init();

    while (1) {
        mpu6050_read_data();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Atualiza os dados a cada 1 segundo
    }
}
