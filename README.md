# KISS LoRa

> For common questions about the KISS gadget, see [the FAQ](FAQ.md).
> 
> Voor veelgestelde vragen over de KISS gadget, zie [de FAQ](FAQ.md).


This repository contains example firmware to customise the KISS LoRa gadget from the E&A fair. If you do not want to customise your KISS gadget, but just want to get it to work with the original firmware, please see [the FAQ](FAQ.md) or the [Commission at Home](COMMISSION_AT_HOME.md) document.


![kiss gadget](KISSLoRa.png)

## Prerequisites

You will need to install a few programs and libraries to be able to upload code to your KISS gadget. This is also neccesary when commissioning your KISS LoRa gadget at home.

### Download this repository

Download the contents of this repository to your computer by clicking [here](https://github.com/YourproductSmarter/KISSLoRa-demo/archive/master.zip) or by clicking on the green button on top of this page: **Clone or download->Download ZIP**. On your computer unzip the downloaded file by following the correct procedure for your operating system.

### Arduino IDE

* Download the latest version of the Arduino IDE from [https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software). If you are using a version of Arduino older than 1.8.1 you also need to update it.

> If your Arduino IDE's interface language is not English, you can change it using the following steps:<br />
> Dutch: **Bestand->Voorkeuren->Taal voor editor->Engels(Engels)**, hierna Arduino IDE opnieuw opstarten.

* Go to **File->Preferences** on the Settings tab, next to **Show verbose output during:**, make sure the checkbox next to **upload** is selected.

### SparkFun AVR boards

* Install the USB drivers for the SparkFun boards, following the steps [for Windows](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/installing-windows) or [for Linux and Mac](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/installing-mac--linux).

* In the Arduino IDE go to **File->Preferences->Additional Boards Manager URLs** and add:
`https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json`

* Go to **Tools->Board->Boards Manager**, search for **SparkFun AVR Boards** and click on **install**.

* Go to **Tools->Board** and select **SparkFun Pro Micro**.

* Go to **Tools->Processor** and select **ATmega32U4 (3.3V, 8MHz)**.


### The Things Network and SoftPWM libraries

If you already have these libraries installed, check if you have the latest versions. Otherwise update them.

* Go to **Sketch->Include Library->Manage Libraries**.

* Search for **TheThingsNetwork** and click on **install**.

* Search for **SoftPWM** and click on **install**.


### The Things Network Console

Applications and devices can be managed via [The Things Network Console](https://console.thethingsnetwork.org/). In the Console you can find your application which is registered via the Commissioning Terminal. 

When uploading the new sketches, you need the `appEui` and `appKey`, you can find both in the Kiss-LoRa application which is registered under your personal account. Go to:
* [The Things Network Console](https://console.thethingsnetwork.org/)
* Click on *Applications* and select the `Kiss LoRa` application
* Click on *Devices* and select your `Kiss LoRa` device
* Go all the way to the bottom, under the `EXAMPLE CODE` you can find the keys you need later on



## Uploading Firmware

> The serial monitor must be closed before uploading code to the device.

1. Switch the KISS LoRa gadget off.
2. Plug it into your computer using a micro USB cable.
3. In the Arduino IDE make sure the correct **Tools->Board** (SparkFun Pro Micro) and **Tools->Processor** (ATmega 32U4, 8MHz) are selected.
4. Switch the device on and make sure the correct **Tools->Port** is selected.
5. Switch the device off again.
6. Press and hold the **Push Button** on the Kiss device.
6. In the Arduino IDE, choose **Sketch->Upload**.
7. As soon as you see `PORTS {} / {} => {}` printed in the output window turn the KISS LoRa on. If you do not see this text, make sure you enabled "Show verbose output during upload" in the Preferences of Arduino.
8. When the upload process is done you can release the push button.

### USB Troubleshooting

It is common to have problems while trying to program Arduino devices, including the KISS LoRa device. In case you experience some issues, a simple Google search will most likely help to find a solution. Some common problems and fixes are listed here:

If you are using Linux, [read this](http://www.arduino.org/learning/getting-started/arduino-ide-on-linux-based-os).

If you are using MAC OS, [read this](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers/all#yosemite).


## [Original firmware](KISSLoRa-demo_app)

In the directory **KISSLoRa-demo_app** you will find the original firmware that was on the KISS LoRa gadget when you received it. If you ever want to upload this firmware back onto the device, have a look at the [README.md in this directory](KISSLoRa-demo_app/README.md). You will also have to upload this firmware onto the gadget if you want to commission or re-commission it at home. 

For the exact steps to commission your KISS LoRa gadget, or go back to the original firmware, see the [Commission at Home](COMMISSION_AT_HOME.md) document.

## [Examples](Examples)

In the Examples directory you will find example firmware files to read the sensors on the board. Have a look at the README in each subdirectory to see an explanation of each example.

## [Bootloader](Bootloader)

In the Bootloader directory you will find the hex file for the bootloader that runs on your KISS LoRa device. In the most cases you will never need this, but if you manage to brick your device, this will be useful to recover.

## [Schematics](Schematics)

The Schematics directory contains the electronic design of the board. It specifically shows how everything is connected. This is useful to have when writing firmware for the KISS LoRa device, to know on which pins the sensors are connected.

The schematic uses the pin names and numbers from the [datasheet of the ATmega32u4](Schematics/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf). When programming this board from within Arduino IDE, we use the Arduino pin names and numbers. See the [PIN_MAPPING](Schematics/PIN_MAPPING.md) document for the translation between these pin numbers and names.
