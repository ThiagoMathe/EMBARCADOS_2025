#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <esp_err.h>

// Definições do barramento I2C
#define I2C_MASTER_NUM I2C_NUM_0        // Número do I2C (I2C0)
#define I2C_MASTER_SDA_IO 21           // GPIO para SDA
#define I2C_MASTER_SCL_IO 20           // GPIO para SCL
#define I2C_MASTER_FREQ_HZ 100000      // Frequência do barramento I2C

// Endereço do MPU6050
#define MPU6050_ADDR 0x68              

// Registradores do MPU6050
#define MPU6050_WHO_AM_I 0x75          // Identificação do dispositivo
#define MPU6050_PWR_MGMT_1 0x6B        // Gerenciamento de energia
#define MPU6050_ACCEL_XOUT_H 0x3B      // Dados do acelerômetro
#define MPU6050_TEMP_OUT_H 0x41        // Dados da temperatura
#define MPU6050_GYRO_XOUT_H 0x43       // Dados do giroscópio

// Sensibilidade do giroscópio
#define GYRO_SENSITIVITY 131.0

/**
 * @brief Inicializa o barramento I2C
 */
void i2c_master_init(void);

/**
 * @brief Inicializa o sensor MPU6050
 */
void mpu6050_init(void);

/**
 * @brief Lê dados do MPU6050 a partir de um registrador específico
 * @param reg_addr Endereço do registrador
 * @param data Ponteiro para armazenar os dados lidos
 * @param len Número de bytes a serem lidos
 * @return esp_err_t Retorna ESP_OK se a leitura for bem-sucedida
 */
esp_err_t read_mpu6050(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Obtém os dados do acelerômetro
 * @param accel_x Ponteiro para armazenar aceleração no eixo X
 * @param accel_y Ponteiro para armazenar aceleração no eixo Y
 * @param accel_z Ponteiro para armazenar aceleração no eixo Z
 */
void read_acceleration(float *accel_x, float *accel_y, float *accel_z);

/**
 * @brief Obtém os dados do giroscópio
 * @param gyro_x Ponteiro para armazenar rotação no eixo X
 * @param gyro_y Ponteiro para armazenar rotação no eixo Y
 * @param gyro_z Ponteiro para armazenar rotação no eixo Z
 */
void read_gyroscope(float *gyro_x, float *gyro_y, float *gyro_z);

/**
 * @brief Lê a temperatura do MPU6050
 * @return float Retorna a temperatura em graus Celsius
 */
float read_temperature(void);

/**
 * @brief Calcula os ângulos de Euler (Roll, Pitch, Yaw) usando integração numérica
 * @param roll Ponteiro para armazenar o ângulo de rolagem (roll)
 * @param pitch Ponteiro para armazenar o ângulo de inclinação (pitch)
 * @param yaw Ponteiro para armazenar o ângulo de guinada (yaw)
 * @param dt Intervalo de tempo entre as leituras
 */
void calculate_euler_angles(float *roll, float *pitch, float *yaw, float dt);

#endif /* MPU6050_H */
