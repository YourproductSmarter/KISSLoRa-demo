# Temperature and Humidity

The KISS LoRa board has a Silicon Labs Si7021 Temperature and Humidity sensor. See [Si7021-A20.pdf](Si7021-A20.pdf) for technical details on this sensor.

Communication with the sensor is done via I2C. This example makes use of a SparkFun library for the Si7021 to setup and read the sensor.