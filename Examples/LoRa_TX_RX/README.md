# LoRa TX and RX example

This example periodically sends a message via The Things Network LoRaWAN network, and polls for a downlink message.

To use this example you have to register a device on [The Things Network Console](https://console.thethingsnetwork.org/applications). 

When registering a device you will need the device's EUI. To obtain the EUI, upload this firmware, run it, and in the Serial Monitor you should see an output looking like the following:

```
-- STATUS
EUI: 0004A30000000000
Battery: 3294
AppEUI: 0000000000000000
DevEUI: 0004A30000000000
Data Rate: 5
RX Delay 1: 1000
RX Delay 2: 2000
```

Copy the code after EUI or DevEUI (they should be the same), consisting of 16 hex character, and use this when registering your device on the TTN console.

After registering your device, at the bottom of the device page, you should see some example code. Copy this code and replace the place-holder code at the top of this example that looks as follows:

```
// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";
```

Also remove line 29, because we do not need this delay anymore, as it was only there to have time to copy the EUI:

```
delay(30000); //remove this after you registered your device
```



When you use this firmware on your KISS gadget, it will poll the network for a downlink message, wait 10 seconds, send the position of the rotary switch and check for downlink messages, wait another 10 seconds, and continue doing the same in a loop.

On the TTN console you should see the rotary switch position arriving on port 2, and the polling messages on port 1. You can send some downlink message using the "Downlink" form on the console. The data you type in the payload field should be HEX characters. When the KISS device receives a downlink message, it will toggle the red LED.
