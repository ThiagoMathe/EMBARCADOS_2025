#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO 9
#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_NUM I2C_NUM_0

#define SSD1306_I2C_ADDRESS 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static const char *TAG = "SSD1306";

// Inicializa o barramento I2C
void i2c_master_init_ssd() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
    
}

// Envia um comando ao SSD1306
void ssd1306_send_command(uint8_t command) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SSD1306_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true); // Byte de controle para comandos
    i2c_master_write_byte(cmd, command, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

// Envia dados para o SSD1306
void ssd1306_send_data(uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SSD1306_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x40, true); // Byte de controle para dados
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

// Inicializa o SSD1306
void ssd1306_init() {
    ssd1306_send_command(0xAE); // Desliga o display
    ssd1306_send_command(0xD5);
    ssd1306_send_command(0x80);
    ssd1306_send_command(0xA8);
    ssd1306_send_command(0x3F);
    ssd1306_send_command(0xD3);
    ssd1306_send_command(0x00);
    ssd1306_send_command(0x40);
    ssd1306_send_command(0x8D);
    ssd1306_send_command(0x14);
    ssd1306_send_command(0x20);
    ssd1306_send_command(0x00);
    ssd1306_send_command(0xA1);
    ssd1306_send_command(0xC8);
    ssd1306_send_command(0xDA);
    ssd1306_send_command(0x12);
    ssd1306_send_command(0x81);
    ssd1306_send_command(0xCF);
    ssd1306_send_command(0xD9);
    ssd1306_send_command(0xF1);
    ssd1306_send_command(0xDB);
    ssd1306_send_command(0x40);
    ssd1306_send_command(0xA4);
    ssd1306_send_command(0xA6);
    ssd1306_send_command(0xAF);
    vTaskDelay(pdMS_TO_TICKS(100)); // Pequeno atraso após inicializar

}

void i2c_init_ssd1306() {
    i2c_master_init_ssd();
    ssd1306_init();
}

// Limpa o display
void ssd1306_clear() {
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_send_command(0xB0 + page);
        ssd1306_send_command(0x00);
        ssd1306_send_command(0x10);
        for (uint8_t col = 0; col < 128; col++) {
            ssd1306_send_data(0x00); // Envia um byte de zeros para limpar
        }
    }
}

// Fonte básica 5x7 ASCII (apenas números e letras para exemplo)
const uint8_t font5x7[36][5] = {
    // Números 0-9
    {0x7E, 0x81, 0xA5, 0x81, 0x7E}, // '0'
    {0x00, 0x82, 0xFF, 0x80, 0x00}, // '1'
    {0xC2, 0xA1, 0x91, 0x89, 0x86}, // '2'
    {0x42, 0x81, 0x89, 0x89, 0x76}, // '3'
    {0x30, 0x28, 0x24, 0xFF, 0x20}, // '4'
    {0x4F, 0x89, 0x89, 0x89, 0x71}, // '5'
    {0x7E, 0x89, 0x89, 0x89, 0x72}, // '6'
    {0x01, 0xE1, 0x19, 0x05, 0x03}, // '7'
    {0x76, 0x89, 0x89, 0x89, 0x76}, // '8'
    {0x46, 0x89, 0x89, 0x89, 0x7E}, // '9'
    
    // Letras minúsculas a-z
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 'a'
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 'b'
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 'c'
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 'd'
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 'e'
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 'f'
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 'g'
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 'h'
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 'i'
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 'j'
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 'k'
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 'l'
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 'm'
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 'n'
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 'o'
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 'p'
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 'q'
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 'r'
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 's'
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 't'
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 'u'
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 'v'
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 'w'
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 'x'
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 'y'
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 'z'
};


void ssd1306_draw_char(char c, uint8_t x, uint8_t y) {
    uint8_t index;

    if (c >= '0' && c <= '9') {
        index = c - '0'; // Números começam no índice 0
    } else if (c >= 'a' && c <= 'z') {
        index = c - 'a' + 10; // Letras começam no índice 10
    } else {
        return; // Caractere não suportado
    }

    ssd1306_send_command(0xB0 + y); // Define página
    ssd1306_send_command(0x00 + (x & 0x0F)); // Coluna baixa
    ssd1306_send_command(0x10 + ((x >> 4) & 0x0F)); // Coluna alta

    for (uint8_t i = 0; i < 5; i++) {
        ssd1306_send_data(font5x7[index][i]); // Envia os bytes do caractere
    }

    ssd1306_send_data(0x00); // Espaço entre caracteres
}


void ssd1306_draw_string(const char *str, uint8_t x, uint8_t y) {
    while (*str) {
        if (x + 6 > SSD1306_WIDTH) { // Se a posição x ultrapassar a largura da tela
            x = 0;                   // Volta para o início da linha
            y++;                      // Vai para a próxima linha

            if (y >= 8) {             // Se ultrapassar o limite de páginas (linhas)
                return;               // Para de escrever para evitar erros
            }
        }

        ssd1306_draw_char(*str, x, y); // Desenha o caractere na posição atual
        x += 6;                         // Move para a próxima posição
        str++;                           // Passa para o próximo caractere
    }
}


void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    
    if (color) {
        ssd1306_buffer[x + (page * SSD1306_WIDTH)] |= (1 << bit); // Define o bit
    } else {
        ssd1306_buffer[x + (page * SSD1306_WIDTH)] &= ~(1 << bit); // Apaga o bit
    }
}

void ssd1306_draw_circle(bool color) {
    uint8_t x0 = SSD1306_WIDTH / 2;  // Centro X (64)
    uint8_t y0 = SSD1306_HEIGHT / 2; // Centro Y (32)
    uint8_t r = (2 * SSD1306_HEIGHT) / 6; // Raio (2/3 da altura / 2)

    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        ssd1306_draw_pixel(x0 + x, y0 + y, color);
        ssd1306_draw_pixel(x0 + y, y0 + x, color);
        ssd1306_draw_pixel(x0 - y, y0 + x, color);
        ssd1306_draw_pixel(x0 - x, y0 + y, color);
        ssd1306_draw_pixel(x0 - x, y0 - y, color);
        ssd1306_draw_pixel(x0 - y, y0 - x, color);
        ssd1306_draw_pixel(x0 + y, y0 - x, color);
        ssd1306_draw_pixel(x0 + x, y0 - y, color);

        y++;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) {
            x--;
            err += 1 - 2 * x;
        }
    }
}

void ssd1306_update_screen() {
    for (uint8_t i = 0; i < 8; i++) {
        ssd1306_send_command(0xB0 + i); // Seleciona a página
        ssd1306_send_command(0x00);
        ssd1306_send_command(0x10);
        for (uint8_t j = 0; j < SSD1306_WIDTH; j++) {
            ssd1306_send_data(ssd1306_buffer[i * SSD1306_WIDTH + j]);
        }
    }
}

void ssd1306_draw_line(int x0, int y0, int x1, int y1, bool color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        ssd1306_draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void ssd1306_draw_triangle(bool color) {
    int x_center = 64; // Centro da tela
    int y_bottom = 55; // Base do triângulo
    int base_half = 42; // Metade da base (2/3 da largura / 2)
    int height = 42; // 2/3 da altura

    int x0 = x_center - base_half; // Canto esquerdo da base
    int x1 = x_center + base_half; // Canto direito da base
    int x2 = x_center;             // Topo do triângulo

    int y0 = y_bottom; // Linha da base
    int y1 = y_bottom;
    int y2 = y_bottom - height; // Altura do triângulo

    // Desenhar as três linhas
    ssd1306_draw_line(x0, y0, x1, y1, color); // Base
    ssd1306_draw_line(x0, y0, x2, y2, color); // Lado esquerdo
    ssd1306_draw_line(x1, y1, x2, y2, color); // Lado direito
}

void ssd1306_draw_rectangle(bool color) {
    // Calcula 2/3 da largura e altura da tela
    uint8_t size_w = (SSD1306_WIDTH * 2) / 3;
    uint8_t size_h = (SSD1306_HEIGHT * 2) / 3;

    // Calcula as coordenadas do canto superior esquerdo para centralizar o quadrado
    uint8_t x = (SSD1306_WIDTH - size_w) / 2;
    uint8_t y = (SSD1306_HEIGHT - size_h) / 2;

    // Desenha os 4 lados do quadrado (linhas)
    ssd1306_draw_line(x, y, x + size_w, y, color); // Linha superior
    ssd1306_draw_line(x + size_w, y + size_h, x, y + size_h, color); // Linha inferior
    ssd1306_draw_line(x + size_w, y, x + size_w, y + size_h, color); // Linha direita
    ssd1306_draw_line(x, y + size_h, x, y, color); // Linha esquerda
}

void ssd1306_draw_square(bool color) {
    // Calcula 2/3 da largura e altura da tela
    uint8_t size = (SSD1306_HEIGHT * 2) / 3;  // Considerando que é um quadrado (lados iguais)

    // Calcula as coordenadas do canto superior esquerdo para centralizar o quadrado
    uint8_t x = (SSD1306_WIDTH - size) / 2;
    uint8_t y = (SSD1306_HEIGHT - size) / 2;

    // Desenha os 4 lados do quadrado (linhas)
    ssd1306_draw_line(x, y, x + size, y, color); // Linha superior
    ssd1306_draw_line(x + size, y, x + size, y + size, color); // Linha direita
    ssd1306_draw_line(x + size, y + size, x, y + size, color); // Linha inferior
    ssd1306_draw_line(x, y + size, x, y, color); // Linha esquerda
}

