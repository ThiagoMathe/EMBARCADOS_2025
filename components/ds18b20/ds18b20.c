#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DS18B20_GPIO 18 // Defina o pino onde está conectado o sensor
#define CMD_CONVERT_T  0x44
#define CMD_READ_SCRATCHPAD 0xBE
#define CMD_SKIP_ROM  0xCC
#define CMD_MATCH_ROM 0x55
#define CMD_READ_ROM  0x33

static const char *TAG = "DS18B20";

// Inicializa o pino GPIO
void ds18b20_init(int gpio) {
    gpio_reset_pin(gpio);
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 1);
}

// Reinicia o barramento 1-Wire e verifica se há sensores conectados
int ds18b20_reset(int gpio) {
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 0);
    esp_rom_delay_us(480); // Delay exato de 480µs
    gpio_set_level(gpio, 1);
    esp_rom_delay_us(70); // Espera 70µs
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    esp_rom_delay_us(410); // Espera mais 410µs

    return gpio_get_level(gpio) == 0; // Se for 0, um sensor respondeu
}

// Escreve um bit no barramento 1-Wire
void ds18b20_write_bit(int gpio, int bit) {
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 0);
    esp_rom_delay_us(bit ? 6 : 60);
    gpio_set_level(gpio, 1);
    esp_rom_delay_us(bit ? 64 : 10);
}

// Lê um bit do barramento 1-Wire
int ds18b20_read_bit(int gpio) {
    int bit;
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 0);
    esp_rom_delay_us(6);
    gpio_set_level(gpio, 1);
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    esp_rom_delay_us(9);
    bit = gpio_get_level(gpio);
    esp_rom_delay_us(55);
    return bit;
}

// Escreve um byte no barramento
void ds18b20_write_byte(int gpio, uint8_t data) {
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(gpio, data & 0x01);
        data >>= 1;
    }
}

// Lê um byte do barramento
uint8_t ds18b20_read_byte(int gpio) {
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        data |= (ds18b20_read_bit(gpio) << i);
    }
    return data;
}

// Verifica CRC
uint8_t ds18b20_crc8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++) {
        uint8_t inByte = data[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ inByte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inByte >>= 1;
        }
    }
    return crc;
}

// Lê a temperatura do DS18B20
float ds18b20_read_temp(int gpio) {
    uint8_t scratchpad[9];
    if (!ds18b20_reset(gpio)) {
        ESP_LOGE(TAG, "Nenhum sensor encontrado!");
        return -1000;
    }

    ds18b20_write_byte(gpio, CMD_SKIP_ROM);
    ds18b20_write_byte(gpio, CMD_CONVERT_T);
    
    // Espera pela conversão sem bloquear
    for (int i = 0; i < 75; i++) {
        vTaskDelay(pdMS_TO_TICKS(10));
        if (ds18b20_read_bit(gpio)) break;
    }
    
    if (!ds18b20_reset(gpio)) {
        ESP_LOGE(TAG, "Erro ao reiniciar sensor!");
        return -1000;
    }

    ds18b20_write_byte(gpio, CMD_SKIP_ROM);
    ds18b20_write_byte(gpio, CMD_READ_SCRATCHPAD);

    for (int i = 0; i < 9; i++) {
        scratchpad[i] = ds18b20_read_byte(gpio);
    }

    // Verifica CRC
    if (ds18b20_crc8(scratchpad, 8) != scratchpad[8]) {
        ESP_LOGE(TAG, "Erro de CRC!");
        return -1000;
    }

    int16_t raw_temp = (scratchpad[1] << 8) | scratchpad[0];
    return raw_temp / 16.0;
}

float ds18b20_read_temp_address(int gpio, uint8_t address[8]) {
    uint8_t scratchpad[9];

    if (!ds18b20_reset(gpio)) {
        ESP_LOGE(TAG, "Nenhum sensor encontrado!");
        return -1000;
    }

    ds18b20_write_byte(gpio, CMD_MATCH_ROM); // Seleciona o sensor específico
    for (int i = 0; i < 8; i++) {
        ds18b20_write_byte(gpio, address[i]); // Envia o endereço do sensor
    }
    
    ds18b20_write_byte(gpio, CMD_CONVERT_T);

    // Espera pela conversão sem bloquear (máx 750 ms)
    for (int i = 0; i < 75; i++) {
        vTaskDelay(pdMS_TO_TICKS(10));
        if (ds18b20_read_bit(gpio)) break;
    }

    if (!ds18b20_reset(gpio)) {
        ESP_LOGE(TAG, "Erro ao reiniciar sensor!");
        return -1000;
    }

    ds18b20_write_byte(gpio, CMD_MATCH_ROM); // Seleciona o sensor novamente
    for (int i = 0; i < 8; i++) {
        ds18b20_write_byte(gpio, address[i]);
    }

    ds18b20_write_byte(gpio, CMD_READ_SCRATCHPAD);

    for (int i = 0; i < 9; i++) {
        scratchpad[i] = ds18b20_read_byte(gpio);
    }

    // Verifica CRC
    if (ds18b20_crc8(scratchpad, 8) != scratchpad[8]) {
        ESP_LOGE(TAG, "Erro de CRC!");
        return -1000;
    }

    int16_t raw_temp = (scratchpad[1] << 8) | scratchpad[0];
    return raw_temp / 16.0;
}
