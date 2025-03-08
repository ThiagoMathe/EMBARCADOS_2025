
# Biblioteca SSD1306 para ESP32

Esta biblioteca permite a interface com o display OLED SSD1306 via protocolo I2C utilizando o framework ESP-IDF no microcontrolador ESP32.

## Configuração

### 1. **Conecte o SSD1306 ao ESP32**

- Conecte o pino `VCC` do SSD1306 ao pino `3.3V` do ESP32.
- Conecte o pino `GND` do SSD1306 ao pino `GND` do ESP32.
- Conecte o pino `SDA` do SSD1306 ao pino `SDA` do ESP32 (por exemplo, GPIO 20).
- Conecte o pino `SCL` do SSD1306 ao pino `SCL` do ESP32 (por exemplo, GPIO 21).

### 2. **Configure o I2C no seu projeto**

No arquivo `main.c` do seu projeto, configure o barramento I2C e inicialize o display:

```c
#include <stdio.h>
#include "ssd1306.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    i2c_init_ssd1306();  // Inicializa o barramento I2C para o display
    ssd1306_clear();      // Limpa o display

    ssd1306_draw_string("Hello, ESP32!", 0, 0);  // Desenha uma string na posição (0,0)

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  // Aguarda 1 segundo
    }
}
```

## Uso

### 1. **Inicialize o SSD1306**

Para inicializar o display, chame a função:

```c
i2c_init_ssd1306();
```

### 2. **Desenhe uma string no display**

Para desenhar uma string, use a função `ssd1306_draw_string`:

```c
ssd1306_draw_string("Texto de exemplo", 0, 0);  // Desenha a string na posição (0, 0)
ssd1306_update_screen();  // Atualiza a tela
```

### 3. **Desenhe formas no display**

Você pode desenhar várias formas no display:

```c
ssd1306_draw_pixel(10, 10, true);  // Desenha um pixel na posição (10, 10)
ssd1306_draw_line(0, 0, 127, 63, true);  // Desenha uma linha de (0,0) até (127,63)
ssd1306_draw_rectangle(true);  // Desenha um retângulo
ssd1306_update_screen();  // Atualiza a tela
```

### 4. **Limpe o display**

Para limpar o display, use:

```c
ssd1306_clear();  // Limpa a tela
ssd1306_update_screen();  // Atualiza a tela
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
    i2c_init_ssd1306();  // Inicializa o display
    ssd1306_clear();      // Limpa o display

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(2000));  // Aguarda 2 segundos

        // Desenha um quadrado
        ssd1306_draw_square(true); 
        ssd1306_update_screen();  // Atualiza a tela
        ssd1306_draw_square(false);  // Limpa o quadrado

        vTaskDelay(pdMS_TO_TICKS(2000));  // Aguarda 2 segundos

        // Desenha um retângulo
        ssd1306_draw_rectangle(true);
        ssd1306_update_screen();  // Atualiza a tela
        ssd1306_draw_rectangle(false);  // Limpa o retângulo
        
        vTaskDelay(pdMS_TO_TICKS(2000));  // Aguarda 2 segundos

        // Desenha um triângulo
        ssd1306_draw_triangle(true);
        ssd1306_update_screen();  // Atualiza a tela
        ssd1306_draw_triangle(false);  // Limpa o triângulo

        vTaskDelay(pdMS_TO_TICKS(2000));  // Aguarda 2 segundos

        // Desenha um círculo
        ssd1306_draw_circle(true);
        ssd1306_update_screen();  // Atualiza a tela
        ssd1306_draw_circle(false);  // Limpa o círculo

        vTaskDelay(pdMS_TO_TICKS(2000));  // Aguarda 2 segundos

        // Exibe uma string
        ssd1306_draw_string("Olá Mundo", 10, 0);
        ssd1306_update_screen();  // Atualiza a tela

        ESP_LOGI(TAG, "Desenhos feitos com sucesso!");
    }
}
```

## Funcionalidades

- **Desenho de texto**: Você pode desenhar texto no display usando a função `ssd1306_draw_string`.
- **Desenho de formas**: A biblioteca oferece funções para desenhar formas como pixels, linhas, retângulos, quadrados, triângulos e círculos.
- **Limpeza do display**: O display pode ser limpo com a função `ssd1306_clear`.
