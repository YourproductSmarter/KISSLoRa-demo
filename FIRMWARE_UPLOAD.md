# Uploading Firmware

It can sometimes be tricky to upload code to the KISS LoRa device. This document tries to explain a procedure to follow to make sure the upload process succeeds.

1. Switch the KISS LoRa device off.
2. Plug it into your computer using a micro USB cable.
3. In the Arduino IDE make sure the correct **Tools->Board** (SparkFun Pro Micro) and **Tools->Processor** (ATmega 32U4, 8MHz) are selected.
4. Switch the device on and make sure the correct **Tools->Port** is selected.
5. Switch the device off again.
6. Press and hold the **Push Button** on the kiss device.
6. In the Arduino IDE, choose **Sketch->Upload**.
7. As soon as you see `PORTS {} / {} => {}` printed in the output window turn KISSLoRa on.
8. When the upload progress past showing `PORTS {} / {} => {}`, you can release the push button.
