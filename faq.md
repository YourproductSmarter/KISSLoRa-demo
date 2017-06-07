## Wat is de precieze functionaliteit van de rotary switch standen op de gadget?

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

## Hoe kom ik erachter of ik dekking heb voor mijn gadget?
De KISSLora gadget communiceert over het LoRa netwerk van The Things Network. Ga naar de website van The Things Network ( [https://www.thethingsnetwork.org/](https://www.thethingsnetwork.org/)) en zie onder het kopje &quot;you are the network&quot; op de home pagina de dekkingskaart.

## Ik heb een vraag over het LoRa netwerk?
Het netwerk waarvan de gadget gebuikt maakt is het LoRa netwerk van The Things Network. Ga naar de website van The Things Network ( [https://www.thethingsnetwork.org/](https://www.thethingsnetwork.org/)) voor meer informatie.

## Hoe kan ik zien dat de gadget werkt?
Dit is op twee manieren te zien, de eerste manier is via de web-app van The Things Network en de tweede manier is via de leds op de gadget.

## Wat is de functionaliteit van de leds op de gadget?
Op de gadget zitten vier leds waaronder een RGB led. De RGB led zit naast de druk knop en geeft de gebruiker visuele feedback over de functies van de rotary switch.

Schuin onder de aan/uit schakelaar zitten twee leds waarvan de linker/groene brand als er een usb kabel is aangesloten en waarvan de rechter/rode brand wanneer de accu wordt opgeladen.

De laatste led zit naast de rotary switch en deze geeft informatie weer over de status van de LoRa verbinding. Deze brand blauw wanneer er data verzonden wordt vanaf de gadget.

## De usb kabel of accu zit aangesloten aan de gadget maar de gadget doet het niet?
Zet de aan/uit schakelaar vlak boven de accu connector aan door deze ter verplaatsen naar &quot;ON&quot;

## De usb kabel en accu zitten aangesloten maar de accu laad niet op?
De gadget moet aan staan om de accu op te kunnen laden. Zet de aan/uit schakelaar op &quot;ON&quot;

## De gadget deed het op de beurs maar thuis niet meer?
Controleer of de accu is opgeladen of voed de gadget via de usb kabel. Controleer of er dekking is in de buurt van de gadget.

## Waar moet ik zijn om verder te kunnen ontwikkelen met de gadget?
De source code en schema&#39;s van de gadget zijn open source en zijn terug te vinden op de &quot;yourproductsmarter&quot; github repository.
**github.com/YourproductSmarter**

## Troubleshooting tabel

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