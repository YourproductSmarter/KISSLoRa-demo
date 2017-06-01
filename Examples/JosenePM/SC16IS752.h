/************************************************************************************
* SC16IS752.h - Header for SC16IS752 dual uart                                      *
* Copyright (c) 2014-2016 Antoine van de Cruyssen. All rights reserved              *
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
#ifndef SC16IS752_h
#define SC16IS752_h
#include <Arduino.h> 
//#include <I2C_Device.h>
#include "I2C.h"

#define RHR          0x00   // Recv Holding Register is 0x00 in READ Mode 
#define THR          0x00   // Xmit Holding Register is 0x00 in WRITE Mode 
#define IER          0x01   // Interrupt Enable Register 
#define IIR          0x02   // Interrupt Identification Register in READ Mode 
#define FCR          0x02   // FIFO Control Register in WRITE Mode 
#define LCR          0x03   // Line Control Register 
#define MCR          0x04   // Modem Control Register 
#define LSR          0x05   // Line status Register 
#define MSR          0x06   // Modem Status Register 
#define SPR          0x07   // ScratchPad Register 
#define TCR          0x06   // Transmission Control Register 
#define TLR          0x07   // Trigger Level Register 
#define TXLVL        0x08   // Xmit FIFO Level Register 
#define RXLVL        0x09   // Recv FIFO Level Register 
#define IODir        0x0A   // I/O P:ins Direction Register 
#define IOState      0x0B   // I/O Pins State Register 
#define IOIntEna     0x0C   // I/O Interrupt Enable Register 
#define IOControl    0x0E   // I/O Pins Control Register 
#define EFCR         0x0F   // Extra Features Control Register 


#define DLL          0x00   // Divisor Latch LSB  0x00 
#define DLH          0x01   // Divisor Latch MSB  0x01 
#define EFR          0x02   // Enhanced Function Register 
#define XON1         0x04
#define XON2         0x05
#define XOFF1        0x06
#define XOFF2        0x07

#define I2CWRITE     0x00 
#define I2CREAD      0x01                                                

#define CHANA     0 
#define CHANB     1                                       

#define UART_8N1    0x03
#define UART_8E1    0x1B
#define BAUD_9600   0x0C
#define BAUD_19200    0x06

#define CO2       0
#define PM        1

class SC16IS752
{
    private:
    int Address;
    public:
        SC16IS752();
        void begin(uint8_t Address, boolean TWI_Begin);
    void setup(uint8_t Channel, uint8_t BaudrateDevisor, uint8_t LineControl);
    void writeUART(uint8_t RegAddr, uint8_t Channel, uint8_t Data);
    uint8_t readUART_Array(uint8_t Channel, uint8_t *Data);
    void setGPIODir(uint8_t Bits);
    uint8_t readGPIO();
    uint8_t checkRxLevel(uint8_t Channel);
    void writeGPIO(uint8_t Data);
};

#endif

