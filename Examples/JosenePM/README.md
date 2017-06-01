# Introduction

At the E&A fair, in the boiler room, a few JosenePM sensors are available for experimentation. This example shows how to read the air quality form these devices, using your KISS LoRa gadget.


## Library usage
Copy both folders in /Libraries to (USER FOLDER)\Arduino\Libraries.
First, we need to enable the PM sensor class, then enable the physical PM sensor.
```
#include <IntemoPM.H>

#define joseneAddress 0x4E

JosenePM JosenePM;

void setup()
{
  Serial.begin(9600);

  JosenePM.begin(joseneAddress, 1);
}
```
JosenePM.begin(uint8_t address, uint8_t channel) sends an initialization command to the sensor, setting it up to allow data transferring.
Then, we need to turn on the PM sensor.
```
...
  JosenePM.begin(joseneAddress, 1);
  JosenePM.powerOn();
}
```
JosenePM.powerOn() turns on the sensor, allowing the device to register the amount of particles in its chamber, which is stored in an internal buffer. To retrieve the data, we use the following method:
```
...
void loop()
{
  uint16_t *data = JosenePM.getData(false);
  Serial.print("Josene PM Data:\nPM10: ");
  Serial.println(data[0]);
  Serial.print("PM2.5: ");
  Serial.println(data[1]);
  delay(2000);
}
```
JosenePM.getData(boolean debug) reads the data from the device, and stores it in an array with a length of two unsigned, sixteen bit integers. The first element in the array is the PM10 value, the second element in the array is the PM2.5 value.

**It is important to make sure a delay exists after reading, with at least 1.8 seconds. Otherwise, you will receive random values.**
