# CayenneLPP example

See [this description](https://www.thethingsnetwork.org/docs/applications/Cayenne/) of what CayenneLPP is, and how you can use it via The Things Network.

This example reads all the sensors on the KISS LoRa device and sends the values using CayenneLPP format, so that you can easily visualise the data on myDevices Cayenne.

Because this example reads all the sensors, it contains all the code that you have previously seen. The only new part is the encoding of the payload using CayenneLPP format.

## Description

The lines in this sketch that you have not yet seen are as follow:

`#include <CayenneLPP.h>`

Include the CayenneLPP library to do the encoding of the data payload for us.

`CayenneLPP lpp(51);`

Create an instance of the LPP library, passing it the maximum size a payload can be that we can transmit. That is 51 bytes for LoRaWAN packets.

```
void sendData(uint8_t port)
{
  lpp.reset();
  lpp.addDigitalInput(1, getButtonPressed());
  lpp.addAnalogInput(2, getRotaryPosition());
  lpp.addRelativeHumidity(3, sensor.getRH());
  lpp.addTemperature(4, sensor.getTemp());
  lpp.addLuminosity(5, get_lux_value());
  
  float x,y,z;
  getAcceleration(&x, &y, &z);
  lpp.addAccelerometer(6, x, y, z);
  
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize(), 2, false, getSF());
}
```

We reset the lpp instance and that will clear all old values from the buffer. Then we add the sensor values to the payload, giving it the "channel number" as first parameter, and the sensor value as second parameter. At the end we send the payload bytes we get from the LPP library, on port 2, not asking for an ack, and on a random spreading factor.

