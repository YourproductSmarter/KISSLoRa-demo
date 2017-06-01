/************************************************************************************
* JosenePM.cpp - Library for JosenePM PM sensor                                     *
* Copyright (c) 2014-2016 Antoine van de Cruyssen. All rights reserved              *
* Edited for KISS LoRa compatibility by Rene Schalks.                               *
*************************************************************************************
* Rev 1.0 - September 2016                                                          *
* - Initial release                                                                 *
*************************************************************************************
* This library is free software; you can redistribute it and/or                     *
* modify it under the terms of the GNU Lesser General Public                        *
* License as published by the Free Software Foundation; either                      *
* version 2.1 of the License, or (at your option) any later version.                *
*                                                                                   *
* This library is distributed in the hope that it will be useful,                   *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                    *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                 *
* Lesser General Public License for more details.                                   *
*                                                                                   *
* You should have received a copy of the GNU Lesser General Public                  *
* License along with this library; if not, write to the Free Software               *
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA        *
************************************************************************************/

#include "IntemoPM.h"

// Constructor
JosenePM::JosenePM()
{
  this->pm = new uint16_t[2];
}

//Initialize communication with the UART.
//Params:
//uint8_t UARTI2CAddress; The I2C address of the UART. This *SHOULD* be 0x4E. If not, check the datasheet for the proper address. It should be specified there.
//uint8_t UARTChannel; The channel the UART device sends data to.
void JosenePM::begin(uint8_t UARTI2CAddress, uint8_t UARTChannel)
{
  SC16IS752::begin(UARTI2CAddress,false);
  SC16IS752::setup(PM,BAUD_9600,UART_8N1);

  SC16IS752::setGPIODir(0xFF);
  //SC16IS752::writeGPIO(0x10);
}

//Turns on the PM sensor.
void JosenePM::powerOn() {
  SC16IS752::writeGPIO(SC16IS752::readGPIO() | 0x10);
}

//Turns off the PM sensor.
void JosenePM::powerOff() {
  SC16IS752::writeGPIO(SC16IS752::readGPIO() & 0xEF);
}

//Retrieves the PM values from the sensor.
//Params:
//boolean dbg; Determines if the function should print the values to the Serial port. Can be ommitted, and is false by default.
//Returns:
//uint16_t[2]; an array of the retrieved PM values. The first index of this array contains the PM10 value, the second index contains the PM2.5 value.
uint16_t * JosenePM::getData(boolean dbg)
{
  uint8_t rx[63]={ };
  uint8_t x = SC16IS752::readUART_Array(PM,rx);
  if(dbg==true)
  {
    for(uint8_t y = 0; y<x; y++) {
      Serial.print (rx[y],HEX);
      Serial.print (",");
    }
  }
  if(x>=10 && rx[0]==0xAA && rx[1]==0xC0 && rx[9]==0xAB) 
  {
    this->pm[0]=((uint16_t)rx[5] << 8) | rx[4];
    this->pm[1]=((uint16_t)rx[3] << 8) | rx[2];
    //pm[1];
    if(dbg==true) {
      Serial.print("PM10:"); Serial.print(this->pm[0]); Serial.print(" ");
      Serial.print("PM2.5:"); Serial.print(this->pm[1]); Serial.print(" ");
      Serial.println("");
    }
  }
  else 
  {
    if(dbg==true) Serial.println(" ?");
  }
  return this->pm;
}

//Checks how much data is currently in the RX buffer.
//Returns:
//uint8_t; the amount of bytes currently in the RX buffer.
uint8_t JosenePM::checkRxLevel(uint8_t channel)
{
  return SC16IS752::checkRxLevel(channel);
}

