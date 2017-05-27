ATMEL ATMEGA32U4 / ARDUINO LEONARDO

D0               PD2                 RXD1/INT2
D1               PD3                 TXD1/INT3
D2               PD1     SDA         SDA/INT1
D3#              PD0     PWM8/SCL    OC0B/SCL/INT0
D4       A6      PD4                 ADC8
D5#              PC6     ???         OC3A/#OC4A
D6#      A7      PD7     FastPWM     #OC4D/ADC10
D7               PE6                 INT6/AIN0

D8       A8      PB4                 ADC11/PCINT4
D9#      A9      PB5     PWM16       OC1A/#OC4B/ADC12/PCINT5
D10#     A10     PB6     PWM16       OC1B/0c4B/ADC13/PCINT6
D11#             PB7     PWM8/16     0C0A/OC1C/#RTS/PCINT7
D12      A11     PD6                 T1/#OC4D/ADC9
D13#             PC7     PWM10       CLK0/OC4A

A0       D18     PF7                 ADC7
A1       D19     PF6                 ADC6
A2       D20     PF5                 ADC5
A3       D21     PF4                 ADC4
A4       D22     PF1                 ADC1
A5       D23     PF0                 ADC0

New pins D14..D17 to map SPI port to digital pins

MISO     D14     PB3                 MISO,PCINT3
SCK      D15     PB1                 SCK,PCINT1
MOSI     D16     PB2                 MOSI,PCINT2
SS       D17     PB0                 RXLED,SS/PCINT0

Connected LEDs on board for TX and RX
TXLED    D30     PD5                 XCK1
RXLED    D17     PB0
HWB              PE2                 HWB
