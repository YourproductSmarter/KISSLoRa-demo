# KISSLoRa-demo_app

Make sure you read the [main README](https://github.com/YourproductSmarter/KISSLoRa-demo#kiss-lora) of this repository, as well as the [Commissioning at Home](https://github.com/YourproductSmarter/KISSLoRa-demo/blob/master/COMMISSION_AT_HOME.md) document.


## Creating your own app

You can either look at the simplistic examples [here](https://github.com/YourproductSmarter/KISSLoRa-demo/tree/master/Examples), or modify this demo application. Have a look around: `switch(app)` (line 282). Choose the app you want to change and make your changes accordingly.

## Notes

1. The demo code uses about 98% of the flash, so you may want to comment out apps to save some space.
2. You can enable warnings: File->Preferences->Compiler warnings: All. Code should compile with only a few warnings.
3. You can enable line numbers: File->Preferences->Display line numbers
4. You can use SPI. Unfortunately, due to lack of pins, the SPI_SS line uses the same pin as the RXLED of the Arduino Leonardo. So either don't use USB serial during SPI or change your USB serial library.
5. During sleep mode the timer used by millis() is not updated, you may try estMillis() instead, but this function is untested.

