## Biblioteca DS18B20 para ESP32

Esta biblioteca permite a leitura dos dados de temperatura do sensor DS18B20 via protocolo 1-Wire utilizando o framework ESP-IDF no microcontrolador ESP32.

### Configuração

1. **Conecte o DS18B20 ao ESP32**:
   - Conecte o pino `VCC` do DS18B20 ao `3.3V` do ESP32.
   - Conecte o pino `GND` do DS18B20 ao `GND` do ESP32.
   - Conecte o pino `DATA` do DS18B20 ao pino escolhido do ESP32 para comunicação 1-Wire (por exemplo, GPIO 9), utilizando um resistor pull-up de 4.7kΩ para `VCC`.

2. **Configure o sensor no seu projeto**:
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

### Uso

1. **Inicialize o DS18B20**:

   ```c
   ds18b20_init(DS18B20_GPIO);
   ```

2. **Leia os dados de temperatura**:

   ```c
   float temperature = ds18b20_read_temp(DS18B20_GPIO);
   ```

3. **Leitura pelo endereço do sensor**:
   Se houver múltiplos sensores DS18B20 no mesmo barramento 1-Wire, podemos especificar o endereço único de cada sensor:

   ```c
   uint8_t sensor_address[8] = {0x28, 0xFF, 0x4C, 0x60, 0x91, 0x16, 0x04, 0x3F};
   float temperature = ds18b20_read_temp_by_address(DS18B20_GPIO, sensor_address);
   ```

### Exemplo Completo

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

