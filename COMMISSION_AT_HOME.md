# Commissioning

During the E&A fair we had a couple of commissioning stations standing around where one could personalise, or commission, your KISS gadget. It is however also possible to commission the device without the commissioning stations. These stations did automate many steps, so at home the process is more complex.

## Prerequisites

* Follow the steps in the main README to install the latest version of the Arduino IDE.
* Also install the USB drivers, SparkFun AVR boards and TheThingsNetwork library.
* [A TTN account](https://account.thethingsnetwork.org/)

## Device info

In the Arduino IDE, go to **File->Examples->TheThingsNetwork->DeviceInfo**. This will open the Device Info example code. On line 7 replace `REPLACE_ME` by `TTN_FP_EU868`. Following the steps in the main readme, **upload** this firmware to the KISS gadget. After uploading the code, got to **Tools->Serial Monitor**. At the bottom of the newly opened serial monitor, change the baudrate to *9600 baud*.

Copy the 16 HEX characters following `EUI:` and use this to register a new device on The Things Network Console.

## Register a new device
[See the TTN documentation](https://www.thethingsnetwork.org/docs/devices/registration.html).

## SendOTAA example

Open the SendOTAA example from **File->Examples->TheThingsNetwork->SendOTAA**. Replace the keys at the top with the keys you got from the TTN Console. After uploading this example, check the console to see if you receive any data. If you don't, you do not have TTN coverage.

## Original Firmware

The previous example proved that we have a working KISS gadget, a working radio module, TTN coverage, and programmed valid keys into the KISS gadget. Now we can upload the original firmware back onto the KISS gadget. Follow the steps in the readme inside the *KISSLoRa-demo_app* directory.


> This readme describes the steps necessary to commission a KISS gadget. Many details still need to be added, but the important steps are already included.
