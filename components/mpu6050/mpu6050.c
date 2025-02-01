#include <stdio.h>
#include "mpu6050.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <esp_err.h>
#include <math.h>

#define I2C_MASTER_NUM I2C_NUM_0        // Número do I2C (I2C0)
#define I2C_MASTER_SDA_IO 21           // GPIO para SDA
#define I2C_MASTER_SCL_IO 20           // GPIO para SCL
#define I2C_MASTER_FREQ_HZ 100000      // Frequência do barramento I2C
#define MPU6050_ADDR 0x68              // Endereço do MPU6050
#define MPU6050_WHO_AM_I 0x75          // Registrador de identificação do MPU6050
#define MPU6050_PWR_MGMT_1 0x6B        // Registrador de gerenciamento de energia
#define MPU6050_ACCEL_XOUT_H 0x3B      // Registrador de saída do eixo X do acelerômetro
#define GYRO_SENSITIVITY     131.0
#define GYRO_XOUT_H          0x43
#define GYRO_YOUT_H          0x45
#define GYRO_ZOUT_H          0x47
#define MPU6050_GYRO_XOUT_H 0x43       // Registrador de saída do eixo X do giroscópio
#define MPU6050_TEMP_OUT_H 0x41        // Registrador de saída da temperatura

void mpu6050_init() {
    uint8_t data[2];

    // Desativa o modo de sleep
    data[0] = MPU6050_PWR_MGMT_1;  // Endereço do registrador
    data[1] = 0x00;                // Valor para desativar o modo de sleep
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, MPU6050_ADDR, data, 2, pdMS_TO_TICKS(100)));

    printf("MPU6050 inicializado");
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

        printf("Acel. X: %d, Acel. Y: %d, Acel. Z: %d", accel_x, accel_y, accel_z);
    } else {
        printf("Erro ao ler dados do MPU6050");
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

float read_temperature() {
    uint8_t data[2]; // 2 bytes para a temperatura
    esp_err_t ret = read_mpu6050(MPU6050_TEMP_OUT_H, data, 2);

    if (ret == ESP_OK) {
        int16_t temp_raw = (int16_t)(data[0] << 8 | data[1]);
        float temperature = (temp_raw / 340.0) + 36.53; // Fórmula do datasheet
        return temperature;
    } else {
        printf("Erro ao ler temperatura do MPU6050\n");
        return -1.0; // Retorna um valor inválido em caso de erro
    }
}

void read_acceleration(float *accel_x, float *accel_y, float *accel_z) {
    uint8_t data[6]; // 6 bytes: 2 para cada eixo (X, Y, Z)
    esp_err_t ret = read_mpu6050(MPU6050_ACCEL_XOUT_H, data, 6);

    if (ret == ESP_OK) {
        int16_t accel_x_raw = (int16_t)((data[0] << 8) | data[1]);
        int16_t accel_y_raw = (int16_t)((data[2] << 8) | data[3]);
        int16_t accel_z_raw = (int16_t)((data[4] << 8) | data[5]);

        // Converter para g (escala de ±2g)
        *accel_x = (accel_x_raw / 16384.0); // 32768 / 2 = 16384
        *accel_y = (accel_y_raw / 16384.0);
        *accel_z = (accel_z_raw / 16384.0);
    } else {
        printf("Erro ao ler dados de aceleração do MPU6050\n");
    }
}

void read_gyroscope(float *gyro_x, float *gyro_y, float *gyro_z) {
    uint8_t data[6]; // 6 bytes: 2 para cada eixo (X, Y, Z)
    esp_err_t ret = read_mpu6050(MPU6050_GYRO_XOUT_H, data, 6);

    if (ret == ESP_OK) {
        int16_t gyro_x_raw = (int16_t)((data[0] << 8) | data[1]);
        int16_t gyro_y_raw = (int16_t)((data[2] << 8) | data[3]);
        int16_t gyro_z_raw = (int16_t)((data[4] << 8) | data[5]);

        // Converter para graus por segundo (escala de ±250°/s)
        *gyro_x = (gyro_x_raw / 131.0); // 32768 / 250 = 131.072
        *gyro_y = (gyro_y_raw / 131.0);
        *gyro_z = (gyro_z_raw / 131.0);
    } else {
        printf("Erro ao ler dados do giroscópio do MPU6050\n");
    }
}

void calculate_euler_angles(float *roll, float *pitch, float *yaw, float dt) {
    float gyro_x, gyro_y, gyro_z;
    read_gyroscope(&gyro_x, &gyro_y, &gyro_z);
    if (gyro_x == 0 && gyro_y == 0 && gyro_z == 0) {
        *roll = 0;
        *pitch = 0;
        *yaw = 0;
    } else {
    // Atualiza os ângulos de Euler usando integração numérica
    *roll += gyro_x * dt;
    *pitch += gyro_y * dt;
    *yaw += gyro_z * dt;
    }
}