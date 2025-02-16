#ifndef DS18B20_H
#define DS18B20_H

#include <stdio.h>
#include <stdint.h>
#include "driver/gpio.h"

#define DS18B20_GPIO 9 // Pino do sensor DS18B20

#define CMD_CONVERT_T  0x44
#define CMD_READ_SCRATCHPAD 0xBE
#define CMD_SKIP_ROM  0xCC
#define CMD_MATCH_ROM 0x55
#define CMD_READ_ROM  0x33

// Inicializa o pino GPIO para comunicação com o sensor
void ds18b20_init(int gpio);

// Reinicia o barramento 1-Wire e verifica a presença do sensor
int ds18b20_reset(int gpio);

// Escreve um bit no barramento 1-Wire
void ds18b20_write_bit(int gpio, int bit);

// Lê um bit do barramento 1-Wire
int ds18b20_read_bit(int gpio);

// Escreve um byte no barramento 1-Wire
void ds18b20_write_byte(int gpio, uint8_t data);

// Lê um byte do barramento 1-Wire
uint8_t ds18b20_read_byte(int gpio);

// Calcula o CRC-8 para validação dos dados
uint8_t ds18b20_crc8(const uint8_t *data, uint8_t len);

// Lê a temperatura do sensor DS18B20
float ds18b20_read_temp(int gpio);

// Lê a temperatura de um sensor específico pelo endereço ROM
float ds18b20_read_temp_address(int gpio, uint8_t address[8]);

#endif // DS18B20_H
