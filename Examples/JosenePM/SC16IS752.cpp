/************************************************************************************
* SC16IS752.cpp - Library for SC16IS752 dual uart                                   *
* Copyright (c) 2014-2016 Antoine van de Cruyssen. All rights reserved              *
* Edited by Rene Schalks to be compatible with KISS LoRa.                           *
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

#include "SC16IS752.h"
//i2c_device Test;

// Constructor
SC16IS752::SC16IS752()
{

}

void SC16IS752::begin(uint8_t Address, boolean TWI_Begin)
{
  this->Address = Address;
  if (TWI_Begin)
  {
    I2c.begin();
  }
  //i2c_device::begin(Address, TWI_Begin);
}

/****************
* Channel Setup *
****************/
void SC16IS752::setup(uint8_t Channel, uint8_t BaudrateDevisor, uint8_t LineControl)
{
  SC16IS752::writeUART(LCR, Channel, 0x80);         // 0x80 to program baud rate divisor 
  SC16IS752::writeUART(DLL, Channel, BaudrateDevisor);  // 12=9600 baud
  SC16IS752::writeUART(DLH, Channel, 0x00);        
  SC16IS752::writeUART(LCR, Channel, 0xBF);         // access EFR register 
  SC16IS752::writeUART(EFR, Channel, 0x10);         // enable enhanced registers 
  SC16IS752::writeUART(LCR, Channel, LineControl);    // 8 data bits, 1 stop bit, no parity 
  SC16IS752::writeUART(FCR, Channel, 0x07);         // reset TXFIFO, reset RXFIFO, enable FIFO mode 
}

/*********************
* Write data to UART *
*********************/
void SC16IS752::writeUART(uint8_t RegAddr, uint8_t Channel, uint8_t Data)
{
  I2c.write(this->Address, ((RegAddr << 3) | (Channel << 1)), Data);
}

/***********************
* Read array from UART *
***********************/
uint8_t SC16IS752::readUART_Array(uint8_t Channel, uint8_t *Data)
{
  uint8_t num = SC16IS752::checkRxLevel(Channel);
  I2c.read(this->Address, ((RHR << 3) | (Channel << 1)), num, Data);
  //i2c_device::read_many(((RHR << 3) | (Channel << 1)), num, Data);
  return num;
}

/******************************
* Get number of bytes in FIFO *
******************************/
uint8_t SC16IS752::checkRxLevel(uint8_t Channel)
{
  byte result;
  I2c.read(this->Address, ((RXLVL << 3) | (Channel << 1)) & 0x7F, 1, &result);
  return result;
  //return i2c_device::read(((RXLVL << 3) | (Channel << 1)) & 0x7F);
}

/************************************
* Set GPIO directions         *
* GPIO0 to GPIO7, 0=input, 1=Output *
************************************/
void SC16IS752::setGPIODir(uint8_t Bits) {
  I2c.write(this->Address, (IOControl << 3), 0x00);
  I2c.write(this->Address, (IODir << 3), Bits);
}

/*******************
* Read GPIO inputs *
* GPIO0 to GPIO7   *
*******************/
uint8_t SC16IS752::readGPIO() {
  //return readI2c(IOState << 3);
  byte result;
  I2c.read(this->Address, IOState << 3, 1, &result);
  return result;
}

/*********************
* Write GPIO outputs *
* GPIO0 to GPIO7     *
*********************/
void SC16IS752::writeGPIO(uint8_t Data) {
  I2c.write(this->Address, (IOState << 3), Data);
}

