/*
  ReadTemperatureHumidity.ino - An example for the KISS LoRa device
  
  The MIT License (MIT)
  
  Copyright (c) 2017 The Things Industries  https://www.thethingsindustries.com/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "SparkFun_Si7021_Breakout_Library.h"

Weather sensor;

void setup() {
  Serial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!Serial && millis() < 10000);

  Serial.println("Starting Temperature & Humidity example");

  //Initialize the I2C Si7021 sensor
  sensor.begin();
}

void loop() {
  // Measure Relative Humidity from the Si7021
  float humidity = sensor.getRH();

  // Measure Temperature from the Si7021
  float tempf = sensor.getTemp();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()

  Serial.print("Temp: ");
  Serial.print(tempf);
  Serial.print("*C, ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%RH");

  delay(1000);
}
