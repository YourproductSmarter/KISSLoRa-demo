/************************************************************************************
* JosenePM.h - Header for JosenePM PM sensor                                        *
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
#ifndef JosenePM_h
#define JosenePM_h
#include <Arduino.h> 
#include "SC16IS752.h"

class JosenePM : public SC16IS752
{
  private:
  //An array of the latest retrieved PM values.
  uint16_t *pm;
  public:
  JosenePM();
  //Initializes communication with the UART.
  //Params:
  //uint8_t UARTI2CAddress; The I2C address of the UART. This *SHOULD* be 0x4E. If not, check the datasheet for the proper address. It should be specified there.
  //uint8_t UARTChannel; The channel the UART device sends data to.
  void begin(uint8_t UARTI2CAddress, uint8_t UARTChannel);
  //Retrieves the PM values from the sensor.
  //Params:
  //boolean dbg; Determines if the function should print the values to the Serial port. Can be ommitted, and is false by default.
  //Returns:
  //uint16_t[2]; an array of the retrieved PM values. The first index of this array contains the PM10 value, the second index contains the PM2.5 value.
  uint16_t * getData(boolean dbg=false);
  //Checks how much data is currently in the RX buffer.
  //Returns:
  //uint8_t; the amount of bytes currently in the RX buffer.
  uint8_t checkRxLevel(uint8_t channel);
  //Turns on the PM sensor.
  void powerOn();
  //Turns off the PM sensor.
  void powerOff();
};

#endif

