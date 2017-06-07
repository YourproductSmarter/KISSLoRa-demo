# Commissioning

During the E&A fair we had a couple of commissioning stations standing around where one could personalise, or commission, your KISS gadget. It is however also possible to commission the device without the commissioning stations. These stations did automate many steps, so at home the process is more complex.

## Prerequisites

Make sure you did everything written under prerequisites in the [main README](https://github.com/YourproductSmarter/KISSLoRa-demo#prerequisites).
* Follow the steps in the [main README](https://github.com/YourproductSmarter/KISSLoRa-demo/blob/master/README.md) to install the latest version of the Arduino IDE.
* Also install the USB drivers, SparkFun AVR boards, TheThingsNetwork library and the SoftPWM library.
* [A TTN account](https://account.thethingsnetwork.org/)

## Device info

In the Arduino IDE, go to **File->Examples->TheThingsNetwork->DeviceInfo**. This will open the Device Info example code. On line 7 replace `REPLACE_ME` by `TTN_FP_EU868`. Following the steps in the main readme, **upload** this firmware to the KISS gadget. After uploading the code, got to **Tools->Serial Monitor**. At the bottom of the newly opened serial monitor, change the baudrate to *9600 baud*.

Copy the 16 HEX characters following `EUI:`. We will use this to register a new device on [The Things Network Console](https://console.thethingsnetwork.org/).

## Obtain your device keys
* Go to [The Things Network Console](https://console.thethingsnetwork.org/).
* Choose **Applications**.
* You should already have an application listed starting with `kiss-lora-`, choose it. Otherwise follow the steps below to register a new application.
* After opening the Application's page, click on **Devices** at the top right.
* You should already have one device registered starting with `kiss-lora-device-`, followed by the 16 hex characters you copied in the previous step. If the hex characters are different, or you do not have any device registered, follow the steps below to register a new device, using the 16 HEX characters from the Device EUI you copied in the previous step.
* After registering a device, or opening the page of an existing one, you should see an **Example Code** section at the bottom of the page. Copy this block of code as you will need this in the next example.

> ### Registering an Application
> * At the top right of the Applications page, click on **add application**.
> * Fill in an Application ID and a Description.
> * Click on **Add application**.

> ### Registering a Device
> * In the Console, from the Application or an application’s Devices screen, click **register device**.
> * For **Device ID**, choose a unique name of lower case, alphanumeric characters and nonconsecutive - and _.
> * For **Device EUI**, copy-paste the one you retrieved from your device.
> * Leave the App Key to be generated.
> * For App EUI, select the generated EUI from the list.
> * Click **Register** to finish.

## Original Firmware

Open the **KISSLoRa-demo_app.ino** from your local downloaded copy of this repository. At the top of this sketch, around line 77, you should see a block of code looking like this:
```
static const bool OVERRIDE = false;
static const char *appEUI = "70B3D57EF0003A0E";
static const char *appKey = "4A9A507DF8EFD742BB07192B29EF8568";
```

* Change OVERRIDE to be `true`.
* Replace the `*appEUI` and `*appKey` lines with the example code you copied from the Device page on The Things Network Console.
* Follow the steps in the main README to upload this sketch to your KISS LoRa gadget.
* After upload is done, you can switch the KISS LoRa off, and back on. Now open a Serial Monitor from the Arduino IDE. Within 20 seconds you should see the following output:
```
--- Resetting RN module
Model: RN2483
Version: 1.0.1
Sending: mac set deveui 0004A30B001F18C9
Sending: mac set adr off
--- Random Seed
-- hwEui read from module: 0004A30B001F18C9
-- seed: 40065
--- I2C Accelerometer initialized
Model: RN2483
Version: 1.0.1
Sending: mac set deveui 0004A30B001F18C9
Sending: mac set adr off
Sending: mac set deveui 0004A30B001F18C9
Sending: mac set appeui 70B3D57EF0003A0E
Sending: mac set appkey 4A9A507DF8EFD742BB07192B29EF8568
Sending: mac save 
Sending: mac set rx2 3 869525000
Sending: mac set ch drrange 1 0 6
Sending: mac set ch dcycle 0 799
Sending: mac set ch dcycle 1 799
Sending: mac set ch dcycle 2 799
Sending: mac set ch dcycle 3 799
Sending: mac set ch freq 3 867100000
Sending: mac set ch drrange 3 0 5
Sending: mac set ch status 3 on
Sending: mac set ch dcycle 4 799
Sending: mac set ch freq 4 867300000
Sending: mac set ch drrange 4 0 5
Sending: mac set ch status 4 on
Sending: mac set ch dcycle 5 799
Sending: mac set ch freq 5 867500000
Sending: mac set ch drrange 5 0 5
Sending: mac set ch status 5 on
Sending: mac set ch dcycle 6 799
Sending: mac set ch freq 6 867700000
Sending: mac set ch drrange 6 0 5
Sending: mac set ch status 6 on
Sending: mac set ch dcycle 7 799
Sending: mac set ch freq 7 867900000
Sending: mac set ch drrange 7 0 5
Sending: mac set ch status 7 on
Sending: mac set pwridx 1
Sending: mac set retx 7
Sending: mac set dr 5
Sending: mac join otaa 
Join accepted. Status: 00000401
DevAddr: 26012F9E
-- Status
EUI: 0004A30B001F18C9
Battery: 3294
AppEUI: 70B3D57EF0003A0E
DevEUI: 0004A30B001F18C9
Data Rate: 5
RX Delay 1: 1000
RX Delay 2: 2000
Selected app: 1
-- Collecting sensordata
--- I2C Selecting SI7021 slave with address: 40
--- I2C Sending command code: F5
--- I2C returned value: 21316
-- Humidity value converted from I2C value: 34
--- I2C Selecting SI7021 slave with address: 40
--- I2C Sending command code: E0
--- I2C returned value: 26988
-- Temperature value converted from I2C value: 25.32
--- AnalogRead returned: 560.00
-- Lux value converted from analog: 318
--- I2C Selecting FXLS8471Q slave with address: 1D
--- I2C Reading from register address: 1
--- I2C Returned value x: 2
--- I2C Reading from register address: 3
--- I2C Returned value y: 1
--- I2C Reading from register address: 5
--- I2C Returned value z: 64
-- hwEui read from module: 0004A30B001F18C9
-- Decoding following data: 
-- Humidity: 34
-- Temperature: 25.32
-- Lux: 318
-- Accelerometer x position: 2
-- Accelerometer y position: 1
-- Accelerometer z position: 64
-- hwEui: 9C81
-- Setting Data Rate
--- SF: 7
-- Sending data
Sending: mac set dr 5
Sending: mac tx uncnf 1 22E3093E01020140819C
Successful transmission
```

If you see `Successful transmission`, everything is working perfectly and you have TTN coverage. On the TTN console you will see your data arriving under the **Data** tab. To draw graphs and widgets from your data, look at the [CayenneLPP example](Examples/CayenneLPP) in this repository.

## Troubleshooting

**Q: I see the following in the Serial Monitor:**
```
Sending: mac join otaa
Join not accepted: denied
Check your coverage, keys and backend status.
```
**A:**
This most likely means that you do not have coverage of The Things Network.

**Q: The output I see in the Serial Monitor looks correct, but I do not see my Data on the TTN Console.**

**A:**
Make sure you copied the keys correctly from the console into the **KISSLoRa-demo_app.ino** sketch before uploading it to the KISS LoRa gadget. Also make sure you set `static const bool OVERRIDE = true;`.
