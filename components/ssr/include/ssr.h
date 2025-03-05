#ifndef SSR_H
#define SSR_H

#include "driver/gpio.h"
#include "driver/ledc.h"

/// Enumeração para definir os modos de operação do SSR
typedef enum {
    SSR_MODE_ON_OFF, // Modo liga/desliga simples (usando GPIO)
    SSR_MODE_PWM     // Modo PWM (controle de potência via modulação por largura de pulso)
} ssr_mode_t;

/// Estrutura para representar um SSR (Solid State Relay) e seus parâmetros de configuração
typedef struct {
    gpio_num_t gpio;     // Pino GPIO ao qual o SSR está conectado
    ssr_mode_t mode;     // Modo de operação do SSR (ON-OFF ou PWM)
    uint8_t pwm_channel; // Canal PWM usado caso o modo seja PWM
    uint32_t pwm_freq;   // Frequência do PWM em Hz (usado no modo PWM)
} ssr_t;

/// Função para inicializar o SSR de acordo com o modo de operação
void ssr_init(ssr_t *ssr);

/// Função para ligar ou desligar o SSR (válido para o modo ON-OFF)
void ssr_set_state(ssr_t *ssr, uint8_t state);

/// Função para definir o duty cycle do SSR (válido para o modo PWM)
void ssr_set_duty(ssr_t *ssr, uint8_t duty);

#endif // SSR_H
