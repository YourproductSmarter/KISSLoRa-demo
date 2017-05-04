/*
File name: KISSLoRa_io.h
Copyright: (C)2017 GEPRO|Electronics
           http://www.gepro-electronics.nl
Author(s): Chris Idema
Purpose  : io-defines for KISSLoRa
*/

#ifndef KISSLoRa_io_h
#define KISSLoRa_io_h 1

#include <avr/io.h>

//#define REVISION_3_0
#define REVISION_3_1

//I/O defines
//BIT refers to the BIT value of the bit of an IO-register (PORTx,PINx or DDRx)  
//The macro _BV is used to convert the bit number to the bit value to allow bitwise logical operations

#define LORA_LED_DDR      DDRB
#define LORA_LED_PORT      PORTB
#define LORA_LED_BIT      _BV(4)

#define RGB_RED_DDR      DDRD
#define RGB_RED_PORT     PORTD
#define RGB_RED_BIT      _BV(6)

#define RGB_GREEN_DDR     DDRD
#define RGB_GREEN_PORT    PORTD
#define RGB_GREEN_BIT     _BV(7)

#ifdef REVISION_3_1
  #define RGB_BLUE_DDR      DDRB
  #define RGB_BLUE_PORT     PORTB
  #define RGB_BLUE_BIT      _BV(7)
#else
  #ifdef REVISION_3_0
    #define RGB_BLUE_DDR      DDRD
    #define RGB_BLUE_PORT     PORTD
    #define RGB_BLUE_BIT      _BV(4)
  #endif
#endif


#define ROTARY_1_PIN      PINC
#define ROTARY_2_PIN      PINC
#define ROTARY_4_PIN      PINB

#define ROTARY_1_BIT      _BV(6)
#define ROTARY_2_BIT      _BV(7)
#define ROTARY_4_BIT      _BV(5)


#ifdef REVISION_3_1
  #define ROTARY_8_PORT     PORTD
  #define ROTARY_8_DDR      DDRD
  #define ROTARY_8_PIN      PIND
  #define ROTARY_8_BIT      _BV(5)
#else
  #ifdef REVISION_3_0
    #define ROTARY_8_PIN      PINE
    #define ROTARY_8_BIT      _BV(6)
  #endif
#endif

#ifdef REVISION_3_1
  #define  BUTTON_IN_PIN     PINE
  #define  BUTTON_IN_BIT     _BV(6)
  //PCINT6
#else
  #ifdef REVISION_3_0
    #define  BUTTON_IN_PIN     PINB
    #define  BUTTON_IN_BIT     _BV(7)
    //PCINT7
  #endif
#endif

#define USB_CABLE_CONNECTED (USBSTA&(1<<VBUS))

#endif
