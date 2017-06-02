/*
  CayenneLPP.ino - use myDevices Cayenne on the KISS LoRa board
  
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

#include <TheThingsNetwork.h>
#include <CayenneLPP.h>
#include <Wire.h>
#include "SparkFun_Si7021_Breakout_Library.h"

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// The KISS device should only be used in Europe
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);
Weather sensor;

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  //start temperature and humidity sensor
  sensor.begin();
}

void loop()
{
  debugSerial.println("-- LOOP");

  sendData(1);

  delay(10000);
}

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
  
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize(), port);
}

uint8_t getButtonPressed()
{
  #define BUTTON_PIN 7
  return !digitalRead(BUTTON_PIN);
}

//Poll the rotary switch
int8_t getRotaryPosition()
{
  #define ROTARY_PIN_0 5
  #define ROTARY_PIN_1 13
  #define ROTARY_PIN_2 9
  #define ROTARY_PIN_3 30

  //Set pins as inputs
  pinMode(ROTARY_PIN_0, INPUT);
  pinMode(ROTARY_PIN_1, INPUT);
  pinMode(ROTARY_PIN_2, INPUT);
  pinMode(ROTARY_PIN_3, INPUT);
  //Disable pullup resistors
  digitalWrite(ROTARY_PIN_0, 0);
  digitalWrite(ROTARY_PIN_1, 0);
  digitalWrite(ROTARY_PIN_2, 0);
  digitalWrite(ROTARY_PIN_3, 0);

  int8_t value = 0;
  if (digitalRead(ROTARY_PIN_0)) {value |= 1 << 0;}
  if (digitalRead(ROTARY_PIN_1)) {value |= 1 << 1;}
  if (digitalRead(ROTARY_PIN_2)) {value |= 1 << 2;}
  if (digitalRead(ROTARY_PIN_3)) {value |= 1 << 3;}
  
  //Set value to -1 if it's higher than 9 - this should never happen but just in case
  if (value > 9)
  {
    value = -1;
  }

  return value;
}

//Get the lux value from the APDS-9007 Ambient Light Photo Sensor
uint16_t get_lux_value()
{
  #define LIGHT_SENSOR_PIN 10

  int digital_value = analogRead(LIGHT_SENSOR_PIN);
  double vlux = digital_value * (2.56/1023.0); //lux value in volts
  double ilux = (vlux / 56) * 1000;            //lux value in micro amperes
  double lux = pow(10, (ilux / 10));           //Convert ilux to Lux value
  return (uint16_t)lux;                        //Return Lux value as value without decimal
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
  #define ACC_RANGE 2
  initAccelerometer();
  setAccelerometerRange(ACC_RANGE);  
  
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
