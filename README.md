# Deauther 5GHz com BW16

Dispositivo desenvolvido para auditoria e testes de ataques em redes Wi-Fi de 5GHz e 2.4GHz, projetado estritamente para uso em ambientes controlados.

## Funcionalidades

- Varredura de pontos de acesso Wi-Fi próximos.
- Seleção de múltiplos alvos.
- Ataques de Desautenticação (Deauth) em redes selecionadas ou em todas as redes visíveis.
- Ataques de Beacon Flood e Beacon + Deauth.
- Modo Monitor para visualização gráfica e contagem de pacotes em tempo real.
- Interface gráfica operada por display OLED e botões de navegação.

## Hardware e Esquema de Ligação

O projeto utiliza a placa **Bw16 (RTL8720DN)**, alimentada por um sistema de bateria recarregável, abrigado em uma case impressa em 3D.

### Componentes Utilizados
- Placa Bw16 (RTL8720DN)
- Display OLED SSD1306 I2C (128x64)
- 3x Botões Push (Tactile Switch)
- Bateria LiPo 650mAh 3.7V
- Módulo Carregador TP4056 (USB Tipo-C)
- Interruptor deslizante micro mini SPDT
- Case customizada impressa em 3D

### Pinagem e Conexões

**1. Botões de Controle**
Os botões operam em modo `INPUT_PULLUP`, ou seja, um lado do botão vai no pino da placa e o outro lado vai ao GND.
- **Botão Cima (UP):** Pino PA27
- **Botão Baixo (DOWN):** Pino PA12
- **Botão OK / Selecionar:** Pino PA13

**2. Display OLED (I2C)**
- **VCC:** 3.3V da Bw16
- **GND:** GND da Bw16
- **SDA:** Pino I2C SDA padrão da Bw16 (geralmente PA26)
- **SCL:** Pino I2C SCL padrão da Bw16 (geralmente PA25)

**3. Sistema de Alimentação e Bateria**
A alimentação torna o dispositivo portátil e é montada da seguinte forma:
- **Bateria 650mAh 3.7V:** Conectada aos terminais `B+` (Positivo) e `B-` (Negativo) do módulo TP4056.
- **Carregamento:** A entrada USB-C do TP4056 é utilizada para recarregar a bateria.
- **Alimentação da Placa:** - O terminal `OUT-` do TP4056 é conectado ao GND da Bw16.
  - O terminal `OUT+` do TP4056 é soldado no **Interruptor Deslizante SPDT** (pino central). O pino lateral do interruptor segue para a entrada de energia da placa Bw16 (pino VIN / 5V), permitindo ligar e desligar a passagem de corrente.

## Imagens do Dispositivo

<p align="center">
  <img src="img/device1.png" width="45%" alt="Demonstração 1 do Dispositivo">
  <img src="img/device2.png" width="45%" alt="Demonstração 2 do Dispositivo">
</p>
