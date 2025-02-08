#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

// Definições do barramento I2C
#define I2C_MASTER_SCL_IO 21       // Pino do SCL
#define I2C_MASTER_SDA_IO 20       // Pino do SDA
#define I2C_MASTER_FREQ_HZ 100000  // Frequência do I2C
#define I2C_MASTER_NUM I2C_NUM_0   // Número do barramento I2C

// Definições do display SSD1306
#define SSD1306_I2C_ADDRESS 0x3C  // Endereço I2C do display
#define SSD1306_WIDTH 128         // Largura do display em pixels
#define SSD1306_HEIGHT 64         // Altura do display em pixels

// Buffer de exibição
extern uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Inicializa o barramento I2C
void i2c_master_init_ssd();

// Envia um comando para o SSD1306
void ssd1306_send_command(uint8_t command);

// Envia dados para o SSD1306
void ssd1306_send_data(uint8_t data);

// Inicializa o SSD1306
void ssd1306_init();

// ssd1306_init + i2c_master_init_ssd
void i2c_init_ssd1306();

// Limpa o display
void ssd1306_clear();

// Desenha um caractere no display
void ssd1306_draw_char(char c, uint8_t x, uint8_t y);

// Desenha uma string no display
void ssd1306_draw_string(const char *str, uint8_t x, uint8_t y);

// Desenha um pixel no display
void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color);

// Desenha um círculo no display
void ssd1306_draw_circle(bool color);

// Atualiza a tela com o buffer atual
void ssd1306_update_screen();

// Desenha uma linha no display
void ssd1306_draw_line(int x0, int y0, int x1, int y1, bool color);

// Desenha um triângulo no display
void ssd1306_draw_triangle(bool color);

// Desenha um retângulo no display
void ssd1306_draw_rectangle(bool color);

// Desenha um quadrado no display
void ssd1306_draw_square(bool color);

#endif // SSD1306_H
