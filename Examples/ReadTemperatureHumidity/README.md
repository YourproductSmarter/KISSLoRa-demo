# Temperature and Humidity

The KISS LoRa board has a Silicon Labs Si7021 Temperature and Humidity sensor. See [Si7021-A20.pdf](Si7021-A20.pdf) for technical details on this sensor.

## Open the example

Open ReadTemperatureHumidity.ino in the Arduino IDE.

## Compile the firmware

In the Arduino IDE, make sure you have selected the board **SparkFun Pro Micro** and the processor **ATmega32U4 (3.3V, 8MHz)**.

Go to **Sketch->Verify/Compile**. After a few seconds you should see "Done Compiling" at the bottom of the Arduino IDE.

## Upload the firmware

In the Arduino IDE under **Tools->Port** you should see a new port being listed after the KISS LoRa device has been plugged into your computer. Plug the device into your computer now using a micro USB cable. Switch it on and choose the new port that appears in the list of ports in the Arduino IDE.

Go to **Sketch->Upload**.

After a few seconds you will most likely see one of the following errors:

`avrdude: error: programmer did not respond to command: exit bootloader`

`the selected serial port does not exist or your board is not connected`

`Connecting to programmer: .avrdude: butterfly_recv(): programmer is not responding`

Follow the following procedure:
1. Turn KISSLoRa off. 
2. Press and hold the push button on KISSLoRa.
3. Click upload in the Arduino IDE.
4. When you see "PORTS {} / {} => {}" printed in the output window turn KISSLoRa on.
5. When the programming starts you can release the push button.