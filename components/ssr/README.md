
# Biblioteca SSR para ESP32

Esta biblioteca permite o controle de relés SSR (Solid State Relay) utilizando o framework ESP-IDF no microcontrolador ESP32.

## Configuração

### 1. **Conectando o SSR ao ESP32**

- Conecte o pino `DC+` do SSR ao pino `3.3V` do ESP32.
- Conecte o pino `DC-` do SSR ao pino `GND` do ESP32.
- Conecte o pino `CH1` do SSR a um pino GPIO do ESP32 para comunicação 1-Wire (exemplo: GPIO 9).

### 2. **Configurando o relé no seu projeto**

No arquivo `main.c` do seu projeto, adicione a seguinte configuração:

```c
#include "ssr.h"

ssr_t ssr = {
    .gpio = GPIO_NUM_9,               // GPIO para controle do SSR
    .mode = SSR_MODE_ON_OFF,          // Modo de operação: SSR_MODE_ON_OFF ou SSR_MODE_PWM
    .pwm_channel = LEDC_CHANNEL_0,    // Canal PWM
    .pwm_freq = 1000                  // Frequência PWM (1 kHz)
};
```

## Uso

### 1. **Inicializando o SSR**

Para inicializar o SSR no seu código, use a função `ssr_init`:

```c
ssr_init(&ssr);
```

### 2. **Enviando comandos para o SSR**

Use a função `ssr_set_state` para controlar o estado do SSR. O valor `1` desliga, e o valor `0` liga o relé.

```c
ssr_set_state(&ssr, 0);  // 0 para ligado, 1 para desligado
```

## Exemplo Completo

Abaixo segue um exemplo completo de como utilizar a biblioteca:

```c
#include "ssr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    ssr_t ssr = {
        .gpio = GPIO_NUM_9,
        .mode = SSR_MODE_ON_OFF,       // Defina o modo: SSR_MODE_ON_OFF ou SSR_MODE_PWM
        .pwm_channel = LEDC_CHANNEL_0, // Canal PWM
        .pwm_freq = 1000               // Frequência PWM de 1 kHz
    };

    ssr_init(&ssr);

    while (1) {
        // **Modo ON/OFF**
        ssr.mode = SSR_MODE_ON_OFF;
        ssr_init(&ssr);

        // Testando o modo ON/OFF
        ssr_set_state(&ssr, 1);          // Desliga o SSR
        vTaskDelay(pdMS_TO_TICKS(1000));

        ssr_set_state(&ssr, 0);          // Liga o SSR
        vTaskDelay(pdMS_TO_TICKS(1000));

        ssr_set_state(&ssr, 1);          // Desliga o SSR
        vTaskDelay(pdMS_TO_TICKS(1000));

        // **Modo PWM**
        ssr.mode = SSR_MODE_PWM;
        ssr_init(&ssr);

        // Testando o modo PWM com variação de duty cycle
        for (int i = 100; i >= 0; i -= 10) {
            ssr_set_duty(&ssr, i);      // Ajusta o duty cycle
            vTaskDelay(pdMS_TO_TICKS(250));
        }

        for (int i = 0; i <= 100; i += 10) {
            ssr_set_duty(&ssr, i);      // Ajusta o duty cycle
            vTaskDelay(pdMS_TO_TICKS(250));
        }
    }
}
```

## Funcionalidades

- **Modo ON/OFF**: Permite ligar e desligar o relé.
- **Modo PWM**: Permite controlar a potência do relé com um sinal PWM, ajustando o duty cycle de 0 a 100%.
