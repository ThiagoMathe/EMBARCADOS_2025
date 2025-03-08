# Projeto de Desenvolvimento de Bibliotecas para Dispositivos Embarcados

## Visão Geral
Este projeto teve como objetivo o desenvolvimento de bibliotecas para dispositivos embarcados utilizando o microcontrolador ESP32 e o framework ESP-IDF. Além disso, as bibliotecas foram aplicadas em um dispositivo final funcional, demonstrando sua utilidade em um cenário prático.

# Equipe do Projeto:

<div style="display: inline_block;">

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/diegoCBorba" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/90847079?v=4" width="100px;" alt="Avatar Matheus"/><br>
        <sub>
          <b>Diego Cardoso</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/jeremiasmarques" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/90351007?v=4" width="100px;" alt="Avatar Rafael"/><br>
        <sub>
          <b>Jeremias Marques</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/MteusBraga" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/93921419?v=4" width="100px;" alt="Avatar Thiago"/><br>
        <sub>
          <b>Mateus Braga</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/ThiagoMathe" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/89406106?v=4" width="100px;" alt="Avatar Thiago"/><br>
        <sub>
          <b>Thiago Matheus</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

</div>

## Etapa 1: Desenvolvimento de Bibliotecas
Foram desenvolvidas bibliotecas para os seguintes módulos/sensores:

### MPU6050 (I2C)
- **Descrição:** Biblioteca para leitura dos dados do acelerômetro e giroscópio via protocolo I2C.
- **Funcionalidades:**
  - Leitura de aceleração nos eixos X, Y e Z.
  - Leitura de rotação nos eixos X, Y e Z.
  - Leitura da Temperatura em Celsius.
  - Cálculo do Ângulo de Euler.

### SSD1306 (I2C)
- **Descrição:** Biblioteca para controle do display OLED SSD1306 via comunicação I2C.
- **Funcionalidades:**
  - Exibição de texto.
  - Desenho de figuras básicas (pontos, linhas, retângulos, quadrados, círculos e triângulos).
  - Suporte a desenho de texto.

### DS18B20 (1-Wire)
- **Descrição:** Biblioteca para leitura de temperatura utilizando o sensor DS18B20 via comunicação 1-Wire.
- **Funcionalidades:**
  - Leitura de temperatura com resolução configurável.
  - Suporte a múltiplos sensores no mesmo barramento.
  - Conversão automática para graus Celsius.

### SSR (Relé de Estado Sólido)
- **Descrição:** Biblioteca para controle de um Relé de Estado Sólido (SSR) com suporte a acionamento ON-OFF e PWM.
- **Funcionalidades:**
  - Controle ON-OFF do relé.
  - Modulação por Largura de Pulso (PWM) para controle de potência.

### S12 (Sensor de Umidade de Solo)
- **Descrição:** Biblioteca para leitura da umidade do solo utilizando o sensor S12 via comunicação analógica.
- **Funcionalidades:**
  - Leitura do nível de umidade do solo.
  - Cálculo da umidade relativa em percentual.

---

## Etapa 2: Desenvolvimento do Dispositivo Final

### Descrição do Projeto
O dispositivo final desenvolvido foi um **Sistema de Monitoramento e Controle de Ambiente**, integrando os sensores e atuadores implementados nas bibliotecas da Etapa 1. O sistema foi projetado para monitorar temperatura, umidade do solo e ambiente, exibir informações em um display OLED e através de uma aplicação web.

- **Repositório do Projeto Web**: [EMBARCADOS-WEB](https://github.com/diegoCBorba/embarcados-web)

### Funcionalidades Implementadas
- Leitura de temperatura via sensor **DS18B20**.
- Leitura de umidade do solo via sensor **S12**.
- Exibição das informações no display **OLED SSD1306**.
- Comunicação via **Wi-Fi** com a aplicação web.
- Monitoramento e controle remoto através da interface web.

### Arquitetura do Sistema
O sistema foi desenvolvido utilizando o **ESP32-S3**, com comunicação entre os módulos via **I2C** e **1-Wire**. A lógica de controle foi implementada em **C utilizando o ESP-IDF**.

O ESP32-S3 se conecta via **Wi-Fi** à aplicação web, permitindo a visualização dos dados.

### Aplicação Web
A aplicação web, desenvolvida em **React**, permitirá a visualização dos dados em tempo real.
