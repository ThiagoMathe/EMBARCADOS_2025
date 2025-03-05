#ifndef S12_H
#define S12_H

#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_err.h"

void s12_init(int analog_pin, int digital_pin); // Inicializa o sensor S12 configurando os pinos analógico e digital
int s12_read_analog(int analog_pin); // Lê o valor da umidade do sensor no modo analógico
bool s12_read_digital(int digital_pin); // Lê o valor da umidade do sensor no modo digital (0 para seco e 1 para umido) 

#endif // S12_H