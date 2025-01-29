#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "esp_err.h"

// Definições do barramento I2C
#define I2C_MASTER_NUM I2C_NUM_0  
#define I2C_MASTER_SDA_IO 21     
#define I2C_MASTER_SCL_IO 20     
#define I2C_MASTER_FREQ_HZ 100000 
#define MPU6050_ADDR 0x68        

// Registradores do MPU6050
#define MPU6050_WHO_AM_I 0x75    
#define MPU6050_PWR_MGMT_1 0x6B  
#define MPU6050_ACCEL_XOUT_H 0x3B 

// Funções de inicialização e leitura de dados
void i2c_master_init(void);
void mpu6050_init(void);
void mpu6050_read_data(void);
esp_err_t read_mpu6050(uint8_t reg_addr, uint8_t *data, size_t len);
void get_accel_data(void);

#endif // MPU6050_H
