# KISS LoRa

This repository includes example software to be used with the KISS LoRa device from the E&A fair.

![alt text](KISSLoRa.png)

## Prerequisites

### Arduino IDE

Download the latest version of the Arduino IDE from [https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software).

> If your Arduino IDE's interface language is not English, you can change it using the following steps:
> Dutch: **Bestand->Voorkeuren->Taal voor editor->Engels(Engels)**, hierna Arduino IDE opnieuw opstarten.

### SparkFun AVR boards

In the Arduino IDE go to **File->Preferences->Additional Boards Manager URLs** and add `https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json`.

Go to **Tools->Board->Boards Manager**, search for **SparkFun AVR Boards** and click on **install**.

Go to **Tools->Board** and select **SparkFun Pro Micro**.

Go to **Tools->Processor** and select **ATmega32U4 (3.3V, 8MHz)**.


### The Things Network and SoftPWM libraries

Go to **Sketch->Include Library->Manage Libraries**.

Search for **TheThingsNetwork** and click on **install**.

Search for **SoftPWM** and click on **install**.


## [Original firmware](KISSLoRa-demo_app)

In the directory **KISSLoRa-demo_app** you will find the original firmware that was on the KISS LoRa device when you received it. If you ever want to flash this firmware back onto the device, have a look at the [README.md in this directory](KISSLoRa-demo_app/README.md).

## [Bootloader](Bootloader)

In the Bootloader directory you will find the hex file for the bootloader that runs on your KISS LoRa device. In the most cases you will never need this, but if you manage to brick your device, this will be useful to recover.

## [Schematics](Schematics)

The Schematics directory contains the electronic design of the board. It specifically shows how everything is connected. This is useful to have when writing firmware for the KISS LoRa device, to know on which pins the sensors are connected.

## [Examples](Examples)

In the Examples directory you will find example firmware files to read the sensors on the board. Have a look at the README in each subdirectory to see an explanation of each example.