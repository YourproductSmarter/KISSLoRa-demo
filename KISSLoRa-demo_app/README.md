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

## FAQ

### Wat is de precieze functionaliteit van de rotary switch standen op de gadget?

0. **Sensor-to-cloud (default)**
        Er wordt periodiek sensor data gestuurd naar de web-app van The Things Network
1. **Sensor-to-cloud met LoRa-packet countdown**
        Countdown tot het volgende data bericht
2. **LoRa Downlink Check**
    Met een druk op de knop wordt er gecheckt of er downlink beschikbaar is
3. **Gravity Sensor**
    De RGB led geeft in kleur de oriëntatie  van de gadget ten opzichte van de zwaartekracht weer
4. **Acceleration Sensor**
    De RGB led visualiseert snelle bewegingen van de gadget aan de hand van accelerometer data
5. **Dice Roller**
    Gebruik de gadget als dobbelsteen door een snelle beweging te maken, de RGB led knippert het aantal keren van de worp
6. **Mood Light**
    RGB led demo waarin de kleur van de led steeds veranderd binnen het RGB spectrum
7. **Night Light**
    Zaklampje die reageert op omgevingslicht, hoe minder licht er wordt gemeten met de omgevingslicht sensor des te feller     brand de RGB led.
8. **Temperatuur Range Check**
    Temperatuur wordt weergeven in drie staten:
    * Onder de 18 graden Celcius, RGB led is blauw
    * Tussen de 18 en de 24 graden Celcius, RGB led is groen
    * Boven de 24 graden Celcius, RGB led is rood
9. **Humidity Sensor**
    Hoe hoger de gemeten luchtvochtigheid des te vaker knippert de RGB led

### Hoe kan ik zien dat de gadget werkt?
Dit is op twee manieren te zien, de eerste manier is via [The Things Network Console](https://console.thethingsnetwork.org/) en de tweede manier is via de leds op de gadget.

### Wat is de functionaliteit van de leds op de gadget?
Op de gadget zitten vier leds waaronder een RGB led. De RGB led zit naast de druk knop en geeft de gebruiker visuele feedback over de functies van de rotary switch.

Schuin onder de aan/uit schakelaar zitten twee leds waarvan de linker/groene brand als er een usb kabel is aangesloten en waarvan de rechter/rode brand wanneer de accu wordt opgeladen.

De laatste led zit naast de rotary switch en deze geeft informatie weer over de status van de LoRa verbinding. Deze brand blauw wanneer er data verzonden wordt vanaf de gadget.


### Troubleshooting tabel

| **Probleem** | **Mogelijke oorzaak** | **Mogelijke oplossing** |
| --- | --- | --- |
| **Alle leds doen het niet** | **Accu is leeg** | **Accu opladen/usb kabel gebruiken** |
| **De groene power led doet het niet** | **Geen power van de usb kabel** | **Check of andere apparaten wel gevoed kunnen worden met de usb kabel** |
| **De rode accu led doet het niet** | **Accu is vol/Accu niet aangesloten** | **Laad de accu op met usb kabel/sluit accu aan** |
| **De blauwe LoRa status led doet het niet** | **Er wordt geen data verzonden** | **Zet de rotary switch in stand 0, 1 of 2 om data te verzenden** |
| **De RGB led doet het niet** | **Er wordt geen functionaliteit gebruikt** | **Verander de stand van de rotary switch** |
|   |   |   |
| **Geen data beschikbaar op de web-app van The Things Network** | **Geen dekking/Niet goed in bedrijf gesteld** | **Check de dekkingskaart van The Things Network/Volg instructies op github om gadget te programmeren** |
|   |   |   |
| **Geen dekking of LoRa werkt niet** | **Buiten bereik van de beschikbare gateways** | **Breid het netwerk van The Things Network uit door een eigen gateway te bouwen of te kopen** |
