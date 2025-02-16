# Biblioteca DS18B20 para ESP32

Esta biblioteca permite a leitura dos dados de temperatura do sensor DS18B20 via protocolo 1-WIRE utilizando o framework ESP-IDF no microcontrolador ESP32.

## Configuração

1. **Conecte o DS18B20 ao ESP32**:
   - Conecte o pino `VCC` do DS18B20 ao `3.3V` do ESP32.
   - Conecte o pino `GND` do DS18B20 ao `GND` do ESP32.
   - Conecte o pino `1-Wire` do DS18B20 ao pino `1-Wire` do ESP32 (por exemplo, GPIO 9).

2. **Configure o I2C no seu projeto**:
   No arquivo `main.c` do seu projeto:
   ```c
    void app_main() {
        ds18b20_init(DS18B20_GPIO);

        while (1) {
            float temperature = ds18b20_read_temp(DS18B20_GPIO);
            if (temperature > -1000) {
                ESP_LOGI(TAG, "Temperatura: %.2f °C", temperature);
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
   ```

## Uso

1. **Inicialize o DS18B20**:
   ```c
    ds18b20_init(DS18B20_GPIO);
   ```

2. **Leia os dados do acelerômetro e giroscópio**:
   ```c
    float temperature = ds18b20_read_temp(DS18B20_GPIO);
   ```

## Exemplo Completo

Aqui está um exemplo completo de como usar a biblioteca:

```c
#include <stdio.h>
#include "ds18b20.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    ds18b20_init(DS18B20_GPIO);

    while (1) {
        float temperature = ds18b20_read_temp(DS18B20_GPIO);
        if (temperature > -1000) {
            ESP_LOGI(TAG, "Temperatura: %.2f °C", temperature);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```