# Biblioteca SSD1306 para ESP32

Esta biblioteca permite a interface com o display OLED SSD1306 via protocolo I2C utilizando o framework ESP-IDF no microcontrolador ESP32.

## Configuração

1. **Conecte o SSD1306 ao ESP32**:
   - Conecte o pino `VCC` do SSD1306 ao `3.3V` do ESP32.
   - Conecte o pino `GND` do SSD1306 ao `GND` do ESP32.
   - Conecte o pino `SDA` do SSD1306 ao pino `SDA` do ESP32 (por exemplo, GPIO 20).
   - Conecte o pino `SCL` do SSD1306 ao pino `SCL` do ESP32 (por exemplo, GPIO 21).

2. **Configure o I2C no seu projeto**:
   No arquivo `main.c` do seu projeto, configure o barramento I2C e inicialize o display:
   ```c
   #include <stdio.h>
   #include "ssd1306.h"
   #include "freertos/FreeRTOS.h"
   #include "freertos/task.h"

   void app_main() {
       i2c_init_ssd1306();
       ssd1306_clear();

       ssd1306_draw_string("Hello, ESP32!", 0, 0);

       while (1) {
           vTaskDelay(pdMS_TO_TICKS(1000));
       }
   }
   ```

## Uso

1. **Inicialize o SSD1306**:
   ```c
   i2c_init_ssd1306();
   ```

2. **Desenhe uma string no display**:
   ```c
   ssd1306_draw_string("Texto de exemplo", 0, 0);
   ssd1306_update_screen();
   ```

3. **Desenhe formas no display**:
   ```c
   ssd1306_draw_pixel(10, 10, true);
   ssd1306_draw_line(0, 0, 127, 63, true);
   ssd1306_draw_rectangle(true);
   ssd1306_update_screen();
   ```

4. **Limpe o display**:
   ```c
   ssd1306_clear();
   ssd1306_update_screen();
   ```

## Exemplo Completo

Aqui está um exemplo completo de como usar a biblioteca:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"

static const char *TAG = "SSD1306";

void app_main() {
    i2c_init_ssd1306();
    ssd1306_clear();

    while (1) {

        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_square(true);
        ssd1306_update_screen();
        ssd1306_draw_square(false);

        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_rectangle(true);
        ssd1306_update_screen();
        ssd1306_draw_rectangle(false);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_triangle(true);
        ssd1306_update_screen();
        ssd1306_draw_triangle(false);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_circle(true);
        ssd1306_update_screen();
        ssd1306_draw_circle(false);

        vTaskDelay(pdMS_TO_TICKS(2000));
        ssd1306_draw_string("ola mundo", 10, 0);


        ESP_LOGI(TAG, "Desenhos feitos com sucesso!");
    }
}
```