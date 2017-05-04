Building KISSLoRa-demo_app:

Tip: If your interface language is not English you can change it using the following steps:
Dutch: Bestand->Voorkeuren->Taal voor editor->Engels(Engels) hierna Arduino IDE opnieuw opstarten.

1) Go to https://www.arduino.cc/en/Main/Software and download the latest Arduino IDE installer*. 
2) Run the installer.
3) Open the sketch KISSLoRa-demo_app/KISSLoRa-demo_app.ino
4) Go to File->Preferences->Additional Boards Manager URLs. Add the following URL and click OK: 
https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json
5) Download the following board package via Tools->Board->Boards Manager: "SparkFun AVR Boards by SparkFun Electronics version 1.1.5". Close window.
6) Select the following board via Tools->Board: "SparkFun Pro Micro".
7) Select the following processor via Tools->Processor: "ATmega32U4 (3.3V, 8MHz)".
8) Download the following libraries via Sketch->Include Library->Manage Libraries: "TheThingsNetwork by The Things Network Version 2.5.2" and "SoftPWM 1.0.0". Close window.
9) Click verify. If there are no errors in the build output you can continue with "Programming firmware".

*Tested on Windows PC with Arduino IDE version 1.8.2.

Programming firmware:

1) Change preferences: File->Preferences->Settings(tab)->Show verbose output during:->upload(check checkbox). You only have to do this once.
2) Turn KISSLoRa off and connect it to the PC using a micro-USB cable. 
3) Press and hold the push button on KISSLoRa.
4) Click upload in the Arduino IDE.
5) When you see "PORTS {} / {} => {}" printed in the output window turn KISSLoRa on.
6) When the programming starts you can release the push button.

Create your own app:

The easiest way to create your own app is to change an existing app. Apps start at: "switch(app)" (line 282). 
Choose the app you want to change and make your changes accordingly.

Notes:
1) The demo code uses about 98% of the flash, so you may want to comment out apps to save some space.
2) You can enable warnings: File->Preferences->Compiler warnings: All. Code should compile with only a few warnings.
3) You can enable line numbers: File->Preferences->Display line numbers
4) You can use SPI. Unfortunately, due to lack of pins, the SPI_SS line uses the same pin as the RXLED of the Arduino Leonardo. So either don't use USB serial during SPI or change your USB serial library.
5) During sleep mode the timer used by millis() is not updated, you may try estMillis() in stead, but this function is untested.

