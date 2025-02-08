#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"

#define DS18B20_GPIO 21 

// Função de reset do sensor DS18B20
void ds18b20_send_reset()
{
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DS18B20_GPIO, 0);
    ets_delay_us(480);
    gpio_set_level(DS18B20_GPIO, 1);
    ets_delay_us(60);
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_INPUT);
    ets_delay_us(420);
}

// Função para ler um único bit do sensor
uint8_t ds18b20_read_bit() {
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DS18B20_GPIO, 0);
    ets_delay_us(2); // Tempo mínimo antes de mudar para entrada
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_INPUT);
    ets_delay_us(10); // Tempo ideal para leitura do bit
    uint8_t bit = gpio_get_level(DS18B20_GPIO);
    ets_delay_us(50); // Aguarda estabilização
    return bit;
}

// Função para ler um byte do sensor
uint8_t ds18b20_read_byte()
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        byte |= (ds18b20_read_bit() << i);
        ets_delay_us(60);
    }
    return byte;
}

// Função para escrever um único bit no sensor
void ds18b20_write_bit(uint8_t bit)
{
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DS18B20_GPIO, 0);
    ets_delay_us(bit ? 5 : 60);
    gpio_set_level(DS18B20_GPIO, 1);
    ets_delay_us(bit ? 55 : 5);
}

// Função para escrever um byte no sensor
void ds18b20_write_byte(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        ds18b20_write_bit(data & (1 << i));
        ets_delay_us(60);
    }
}

// Função para obter a temperatura do sensor
float ds18b20_get_temperature()
{
    ds18b20_send_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Start temperature conversion
    vTaskDelay(pdMS_TO_TICKS(1000)); // Aguardar 1 segundo para conversão

    ds18b20_send_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0xBE); // Ler scratchpad

    uint8_t lsb = ds18b20_read_byte();
    uint8_t msb = ds18b20_read_byte();

    // 🔍 Debug: Verificar valores lidos
    printf("LSB: %02X, MSB: %02X\n", lsb, msb);

    // Combina os dois bytes em um valor de 16 bits (raw_temp)
    int16_t raw_temp = (msb << 8) | lsb;

    // Se o MSB indicar valor negativo (bit de sinal 1), ajusta o valor para complemento de dois
    if (msb & 0x80) {  // Verifica se o MSB é 1 (indica número negativo)
        raw_temp |= 0xF000;  // Ajusta o valor para o complemento de dois (signo negativo)
    }

    // Calcula a temperatura dividindo por 16.0
    float temperature = raw_temp / 16.0;

    // 🔍 Verificar se a temperatura está em uma faixa válida
    if (temperature < -55.0 || temperature > 125.0) {
        printf("Valor de temperatura inválido: %.2f°C\n", temperature);
        return -0.06;  // Retorna um valor padrão de erro
    }

    printf("Temperatura válida: %.2f°C\n", temperature);
    return temperature;
}


