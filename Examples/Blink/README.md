# Blink

This example will consecutively flash all the LEDs on the KISS LoRa device.

The board has one RGB LED, of which the three different colours can be addresses independently. It therefore looks like three different LEDs in one package. The RGB LED is located close to the push button.

There is also a separate blue LED located close to the rotary encoder. This is also called the LoRa status LED.

## Connections
The LEDs are connected to the following output pins on the microcontroller:

| name | Arduino Pin Number |
| ---- | ------------------ |
| RGB red   | 12 |
| RGB green | 6  |
| RGB blue  | 4  |
| LoRa LED  | 8  |

## Description of code segments

```
#define LED_RED   12
#define LED_GREEN 6
#define LED_BLUE  4
#define LED_LORA  8
```

We define aliases for the pin numbers to which the LEDs are connected. This helps to make the code more readable.

`pinMode(LED_RED, OUTPUT);`

This line means that the pin to which the red led is connected to the microcontroller should be configured as an output.

`digitalWrite(LED_RED, LOW);`

When making a output pin low, it means there will be 0 volt on it. Because of the way the LEDs are wired, making the output low will switch the LED on. On the other hand, writing `HIGH` to the pin will switch the LED off.
