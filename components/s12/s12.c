#include "s12.h"
#include "esp_log.h"

static const char *TAG = "S12_SENSOR";

// Inicializa o sensor S12 configurando os pinos analógico e digital
void s12_init(int analog_pin, int digital_pin) { 
    // Configuração do ADC para leitura analógica
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(analog_pin, ADC_ATTEN_DB_11);

    // Configuração do GPIO para leitura digital
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << digital_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

// Lê o valor da umidade do sensor no modo analógico
int s12_read_analog(int analog_pin) {
    int value = adc1_get_raw(analog_pin);
    ESP_LOGI(TAG, "Analogico: %d", value);
    return value;
}

// Lê o valor da umidade do sensor no modo digital (0 para seco e 1 para umido)
bool s12_read_digital(int digital_pin) {
    bool state = gpio_get_level(digital_pin);
    ESP_LOGI(TAG, "Digital: %d", state);
    return state;
}

