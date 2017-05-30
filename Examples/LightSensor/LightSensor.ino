/*
  LightSensor.ino - An example for the KISS LoRa device
  
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

#define LIGHT_SENSOR_PIN 10

void setup() {
  Serial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!Serial && millis() < 10000);

  Serial.println("Starting the light sensor example");
}

void loop() {
  Serial.print("Ambient light: ");
  Serial.print(get_lux_value());
  Serial.println("lux");
  delay(1000);
}

//Get the lux value from the APDS-9007 Ambient Light Photo Sensor
uint16_t get_lux_value()
{
  int digital_value = analogRead(LIGHT_SENSOR_PIN);
  double vlux = digital_value * (2.56/1023.0); //lux value in volts
  double ilux = (vlux / 56) * 1000;            //lux value in micro amperes
  double lux = pow(10, (ilux / 10));           //Convert ilux to Lux value
  return (uint16_t)lux;                        //Return Lux value as value without decimal
}
