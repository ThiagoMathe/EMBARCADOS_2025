## Biblioteca DS18B20 para ESP32

Esta biblioteca permite a leitura dos dados de temperatura do sensor DS18B20 via protocolo 1-Wire utilizando o framework ESP-IDF no microcontrolador ESP32. Além disso, foi implementado um filtro de média móvel para suavizar as leituras de temperatura, melhorando a precisão e estabilidade das medições.

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
           float raw_temp = ds18b20_read_temp(DS18B20_GPIO);
           if (raw_temp > -1000) {
               float filtered_temp = ds18b20_moving_average(raw_temp);
               ESP_LOGI(TAG, "Temperatura bruta: %.2f °C, Temperatura filtrada: %.2f °C", raw_temp, filtered_temp);
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

3. **Aplique o filtro de média móvel**:
   Para suavizar as leituras de temperatura, utilize a função `ds18b20_moving_average`:

   ```c
   float filtered_temp = ds18b20_moving_average(temperature);
   ```

4. **Leitura pelo endereço do sensor**:
   Se houver múltiplos sensores DS18B20 no mesmo barramento 1-Wire, podemos especificar o endereço único de cada sensor:

   ```c
   uint8_t sensor_address[8] = {0x28, 0xFF, 0x4C, 0x60, 0x91, 0x16, 0x04, 0x3F};
   float temperature = ds18b20_read_temp_address(DS18B20_GPIO, sensor_address);
   float filtered_temp = ds18b20_moving_average(temperature);
   ```

### Exemplo Completo

Aqui está um exemplo completo de como usar a biblioteca com o filtro de média móvel:

```c
#include <stdio.h>
#include "ds18b20.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    ds18b20_init(DS18B20_GPIO);

    while (1) {
        float raw_temp = ds18b20_read_temp(DS18B20_GPIO);
        if (raw_temp > -1000) {
            float filtered_temp = ds18b20_moving_average(raw_temp);
            ESP_LOGI(TAG, "Temperatura bruta: %.2f °C, Temperatura filtrada: %.2f °C", raw_temp, filtered_temp);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```

### Detalhes do Filtro de Média Móvel

O filtro de média móvel armazena as últimas `MOVING_AVERAGE_WINDOW` leituras de temperatura (definido como 10 por padrão) e calcula a média dessas leituras para suavizar as flutuações. Isso é útil para reduzir ruídos e obter medições mais estáveis.

- **Tamanho da Janela**: O tamanho da janela da média móvel pode ser ajustado alterando o valor de `MOVING_AVERAGE_WINDOW` no arquivo `DS18B20.h`.
- **Uso**: A função `ds18b20_moving_average` deve ser chamada após cada leitura de temperatura para aplicar o filtro.
