Aqui está o **README.md** para o repositório da biblioteca do **S12 (Sensor de Umidade de Solo)**. Ele fornece uma visão geral da biblioteca, instruções de uso e exemplos de código.

---

# **Biblioteca S12 (Sensor de Umidade de Solo)**

Esta biblioteca foi desenvolvida para facilitar a leitura de umidade do solo utilizando o sensor **S12**. O sensor pode operar em dois modos:
1. **Modo Analógico**: Fornece um valor proporcional à umidade do solo (0 a 4095 para ESP32 com ADC de 12 bits).
2. **Modo Digital**: Fornece um valor binário indicando se o solo está úmido (1) ou seco (0).

---

## **Funcionalidades**

- **Inicialização do Sensor**:
  - Configura o pino analógico para leitura do ADC e o pino digital para leitura de estado.
  - Função:
    - `s12_init(int analog_pin, int digital_pin)`: Inicializa o sensor, configurando os pinos analógico e digital.

- **Leitura Analógica**:
  - Lê o valor analógico do sensor, que varia conforme a umidade do solo.
  - Função:
    - `s12_read_analog(int analog_pin)`: Retorna o valor lido pelo ADC (0 a 4095).

- **Leitura Digital**:
  - Lê o estado digital do sensor, indicando se o solo está úmido ou seco.
  - Função:
    - `s12_read_digital(int digital_pin)`: Retorna `true` (1) para solo úmido e `false` (0) para solo seco.

---

## **Instalação**

1. Adicione a biblioteca ao seu projeto ESP-IDF:
   - Copie a pasta `s12` para o diretório `components` do seu projeto.

2. Inclua a biblioteca no seu código:
   ```c
   #include "s12.h"
   ```

---

## **Uso Básico**

### **Inicialização e Leitura**

1. Inicialize o sensor no `app_main`:
   ```c
   void app_main() {
       int analog_pin = ADC1_CHANNEL_6; // Pino analógico (GPIO 34)
       int digital_pin = GPIO_NUM_5;   // Pino digital (GPIO 5)

       s12_init(analog_pin, digital_pin); // Inicializa o sensor
   }
   ```

2. Leia os valores analógico e digital:
   ```c
   void app_main() {
       int analog_pin = ADC1_CHANNEL_6; // Pino analógico (GPIO 34)
       int digital_pin = GPIO_NUM_5;   // Pino digital (GPIO 5)

       s12_init(analog_pin, digital_pin); // Inicializa o sensor

       while (1) {
           int analog_value = s12_read_analog(analog_pin); // Lê o valor analógico
           bool digital_state = s12_read_digital(digital_pin); // Lê o estado digital

           printf("Valor Analógico: %d\n", analog_value);
           printf("Estado Digital: %s\n", digital_state ? "Úmido" : "Seco");

           vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo
       }
   }
   ```

---

## **Estrutura do Código**

### **Arquivo `s12.h`**
Define as funções e constantes da biblioteca:
- `s12_init`: Inicializa o sensor.
- `s12_read_analog`: Lê o valor analógico.
- `s12_read_digital`: Lê o valor digital.

### **Arquivo `s12.c`**
Implementa as funções da biblioteca:
- Configura o ADC e o GPIO.
- Realiza a leitura dos valores analógico e digital.

---

## **Exemplo Completo**

Aqui está um exemplo completo de uso da biblioteca:

```c
#include <stdio.h>
#include "s12.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    int analog_pin = ADC1_CHANNEL_6; // Pino analógico (GPIO 34)
    int digital_pin = GPIO_NUM_5;   // Pino digital (GPIO 5)

    s12_init(analog_pin, digital_pin); // Inicializa o sensor

    while (1) {
        int analog_value = s12_read_analog(analog_pin); // Lê o valor analógico
        bool digital_state = s12_read_digital(digital_pin); // Lê o estado digital

        printf("Valor Analógico: %d\n", analog_value);
        printf("Estado Digital: %s\n", digital_state ? "Úmido" : "Seco");

        vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo
    }
}
```
