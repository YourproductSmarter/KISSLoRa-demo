# Push Button Switch

The KISS LoRa board has one push button switch. This example polls the switch every second and prints out the state. It also uses an interrupt to detect when the switch was pressed and toggles the red LED.

In the schematic we can see that the switch will connect the pin on the microcontroller to 0V when it is pressed, but connected via a resistor to 3V3 when it is not pressed. This means that we will read a 0 on the digital input pin on the microcontroller when the button is pressed. On the other hand we will read a 1 when the button is not pressed.

The button is also connected to a pin that supports an interrupt. This means that a specific Interrupt Service Routine (ISR) can be executed when the state of the input pin changes. This is used to toggle the LED when the button is pressed.