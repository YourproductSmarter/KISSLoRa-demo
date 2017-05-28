/*
  Accelerometer.ino - An example for the KISS LoRa device
  
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

#include <Wire.h>

#define ACC_RANGE 2 //Set up to read the accelerometer values in range -2g to +2g - valid ranges: ±2G,±4G or ±8G

void setup() {
  Serial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!Serial && millis() < 10000);

  Serial.println("Starting the accelerometer example");
  
  Wire.begin();
  initAccelerometer();
  setAccelerometerRange(ACC_RANGE);     
}

void loop() {float x,y,z;
  getAcceleration(&x, &y, &z);

  Serial.print("Acceleration:\tx=");
  Serial.print(x);
  Serial.print("g\n\t\ty=");
  Serial.print(y);
  Serial.print("g\n\t\tz=");
  Serial.print(z);
  Serial.println("g");
  Serial.println("---");
  delay(1000);
}

//Write one register to the acceleromter
void writeAccelerometer(unsigned char REG_ADDRESS, unsigned char DATA)
{
  Wire.beginTransmission(0x1D);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}

//Read one register from the accelerometer
uint8_t readAccelerometer(unsigned char REG_ADDRESS)
{
  uint8_t resp;
  Wire.beginTransmission(0x1D);
  Wire.write(REG_ADDRESS);
  Wire.endTransmission(false);
  Wire.requestFrom(0x1D, 1);
  resp = Wire.read();
  return resp;
}

//Configure and activate the FXLS8471Q Accelerometer 
static void initAccelerometer(void)
{
  //Check if the chip responds to the who-am-i command, should return 0x6A (106)
  if (readAccelerometer(0x0D) == 106)
  {
    //Configure FXLS8471Q CTRL_REG1 register
    //Set f_read bit to activate fast read mode
    //Set active bit to put accelerometer in active mode
    writeAccelerometer(0x2A, 0x03);  
  }
  else
  {
    Serial.println(F("--- I2C Accelerometer not initialized"));
  } 
}

//Set the range of the FXLS8471Q Accelerometer, valid ranges: 2G,4G or 8G
static void setAccelerometerRange(uint8_t range_g)
{
  uint8_t range_b;
  switch(range_g)
  {
    case 2:
      range_b = 0;
    break;
    case 4:
      range_b = 1;
    break;
    case 8:
      range_b = 2;
    break;
    default:
      range_b = 0;
    break;
  }
  writeAccelerometer(0x0E, range_b);
}

//Read the acceleration from the accelerometer
void getAcceleration(float *x, float *y, float *z)
{
  // Resource: https://github.com/sparkfun/MMA8452_Accelerometer/blob/master/Libraries/Arduino/src/SparkFun_MMA8452Q.cpp
  // Read the acceleration from registers 1 through 6 of the MMA8452 accelerometer.
  // 2 registers per axis, 12 bits per axis.
  // Bit-shifting right does sign extension to preserve negative numbers.
  *x = ((short)(readAccelerometer(1)<<8 | readAccelerometer(2))) >> 4;
  *y = ((short)(readAccelerometer(3)<<8 | readAccelerometer(4))) >> 4;
  *z = ((short)(readAccelerometer(5)<<8 | readAccelerometer(6))) >> 4;

  // Scale 12 bit signed values to units of g. The default measurement range is ±2g.
  // That is 11 bits for positive values and 11 bits for negative values.
  // value = (value / (2^11)) * 2
  *x = (float)*x / (float)(1<<11) * (float)(ACC_RANGE);
  *y = (float)*y / (float)(1<<11) * (float)(ACC_RANGE);
  *z = (float)*z / (float)(1<<11) * (float)(ACC_RANGE);
}
