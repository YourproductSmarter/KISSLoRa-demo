# Accelerometer

The KISS LoRa board has an FXLS8471Q accelerometer. See [FXLS8471Q.pdf](FXLS8471Q.pdf) for technical details on this sensor.

Communication with the accelerometer is also via the I2C bus, just like with the temperature and humidity sensor. The difference is that we do not have a library to handle setting up and reading values from the accelerometer. This makes this example more complex.

I2C communication is handled by the Wire library. At startup we initialise the accelerometer by firstly checking if the device responds correctly via I2C. We then change a few configuration bits to allow constant measurements. We also configure it to read acceleration in the range -2g to +2g.

At this stage it is important to remember that gravity causes a 1g acceleration toward the centre of the earth. This means that we can only measure an extra 1g downward acceleration, but a 3g upward acceleration. In all other axes we can measure 2g of acceleration. This range is good enough for most uses, but if you know that you need a greater range, but at a lower resolution, you can change the range configuration to either -4g to +4g or -8g to +8g.