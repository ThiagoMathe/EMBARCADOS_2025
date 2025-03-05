#include "ssr.h"
#include "esp_log.h"

#define TAG "SSR_CONTROL"

// Função de inicialização do SSR (Solid State Relay)
void ssr_init(ssr_t *ssr) {
    if (ssr->mode == SSR_MODE_ON_OFF) {
        // Configura o GPIO como saída para controle ON-OFF
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << ssr->gpio),  // Define qual pino será configurado
            .mode = GPIO_MODE_OUTPUT,             // Define o modo como saída
            .pull_up_en = GPIO_PULLUP_DISABLE,    // Desativa o pull-up interno
            .pull_down_en = GPIO_PULLDOWN_DISABLE,// Desativa o pull-down interno
            .intr_type = GPIO_INTR_DISABLE        // Desativa interrupções
        };
        gpio_config(&io_conf);
        ESP_LOGI(TAG, "SSR inicializado no modo ON-OFF");

    } else if (ssr->mode == SSR_MODE_PWM) {
        // Configuração do PWM usando LEDC
        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,   // Define o modo de velocidade do LEDC
            .timer_num = LEDC_TIMER_0,          // Seleciona o timer 0
            .duty_resolution = LEDC_TIMER_10_BIT, // Resolução de 10 bits (0-1023)
            .freq_hz = ssr->pwm_freq,           // Frequência do PWM
            .clk_cfg = LEDC_AUTO_CLK            // Configuração automática do clock
        };
        ledc_timer_config(&ledc_timer);

        ledc_channel_config_t ledc_channel = {
            .gpio_num = ssr->gpio,            // Define o pino GPIO usado para PWM
            .speed_mode = LEDC_LOW_SPEED_MODE,// Mesmo modo de velocidade do timer
            .channel = ssr->pwm_channel,      // Define o canal PWM
            .timer_sel = LEDC_TIMER_0,        // Usa o timer 0
            .duty = 0,                        // Inicializa com duty cycle 0%
            .hpoint = 0                       // Define o ponto inicial do PWM
        };
        ledc_channel_config(&ledc_channel);
        
        ESP_LOGI(TAG, "SSR inicializado no modo PWM, freq: %lu Hz", (unsigned long) ssr->pwm_freq);
    }
}

// Função para ligar ou desligar o SSR no modo ON-OFF
void ssr_set_state(ssr_t *ssr, uint8_t state) {
    if (ssr->mode == SSR_MODE_ON_OFF) {
        gpio_set_level(ssr->gpio, state); // Define o nível lógico do pino
        ESP_LOGI(TAG, "SSR %s", state ? "LIGADO" : "DESLIGADO");
    }
}

// Função para ajustar o duty cycle do SSR no modo PWM
void ssr_set_duty(ssr_t *ssr, uint8_t duty) {
    if (ssr->mode == SSR_MODE_PWM) {
        uint32_t duty_scaled = (duty * 1023) / 100; // Converte o duty cycle para escala de 10 bits (0-1023)
        ledc_set_duty(LEDC_LOW_SPEED_MODE, ssr->pwm_channel, duty_scaled);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, ssr->pwm_channel);
        ESP_LOGI(TAG, "SSR PWM Duty: %d%%", duty);
    }
}
