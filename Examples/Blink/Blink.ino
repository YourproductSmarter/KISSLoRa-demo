/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/

//#define LED_BLUE PB7
#define LED_BLUE PD4
#define LED_RED PD6
#define LED_GREEN PD7
#define LED_LORA PB4

// For Bitvise Operation Simplification Defines
#define CLR(port,pin)  (PORT ## port &= ~(1<<pin))
#define SET(port,pin) (PORT ## port |=  (1<<pin))
#define TOGL(port,pin)  (PORT ## port ^=  (1<<pin))
#define READ(port,pin)  (PIN  ## port &   (1<<pin))
#define OUT(port,pin) (DDR  ## port |=  (1<<pin))
#define IN(port,pin)  (DDR  ## port &= ~(1<<pin))
#define PUP(port,pin) (DDR  ## port &= ~(1<<pin));(PORT ## port |=  (1<<pin))
#define BIT(x)      _BV(x)

// the setup function runs once when you press reset or power the board
void setup() {
  OUT(B,4);
}

// the loop function runs over and over again forever
void loop() {
  SET(B,4);
  delay(1000);                       // wait for a second
  CLR(B,4);
  delay(1000);                       // wait for a second
}
