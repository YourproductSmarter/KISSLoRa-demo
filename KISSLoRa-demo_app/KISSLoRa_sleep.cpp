#include "KISSLoRa_sleep.h"
#include "KISSLora_io.h"

//http://playground.arduino.cc/Learning/ArduinoSleepCode
//http://playground.arduino.cc/Code/Timer1
//http://forum.arduino.cc/index.php?topic=38046.0
//http://donalmorrissey.blogspot.nl/2011/11/sleeping-arduino-part-4-wake-up-via.html
//https://oscarliang.com/arduino-timer-and-interrupt-tutorial/
//https://arduino-info.wikispaces.com/Timers-Arduino

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <Arduino.h>

//all peripherals disabled except timer0 and uart1
//supply 3.8V 100mA
//series current meter
//rotary switch at 0(0 pins high, so 0*0.33mA)
//black tape on light sensor
//3.77 V at connector
//current consumption: 13.16mA off, 16.58 max

//all peripherals disabled except timer0
//supply 3.8V 100mA
//series current meter
//rotary switch at 0(0 pins high, so 0*0.33mA)
//black tape on light sensor
//3.77 V at connector
//current consumption: 13.07mA off, 16.48 max

//all peripherals disabled except timer0, disabled on chip debugging
//supply 3.8V 100mA
//series current meter
//rotary switch at 0(0 pins high, so 0*0.33mA)
//black tape on light sensor
//3.77 V at connector
//current consumption: 13.05mA off, 16.46 max(redone measurement without on chip debugging enabled and got same results, so difference is possbily a measurement error)

//all peripherals disabled except timer1
//sleep mode: idle
//supply 3.8V 100mA
//series current meter
//rotary switch at 0(0 pins high, so 0*0.33mA)
//black tape on light sensor
//3.77 V at connector
//current consumption: 10.65mA off, 14.02 on

//all peripherals enabled
//supply 3.8V 100mA
//series current meter
//rotary switch at 0(0 pins high, so 0*0.33mA)
//black tape on light sensor
//3.77 V at connector
//current consumption: 13.80mA off, 17.22 max

//all peripherals disabled
//sleep mode: power down
//supply 3.8V 100mA
//series current meter
//rotary switch at 0(0 pins high, so 0*0.33mA)
//black tape on light sensor
//3.77 V at connector
//current consumption: 8.72mA off, 12.04 max

/*
static volatile uint8_t pin_interrupt_flag;

static void KISSLoRa_led_toggle(void){

  detachInterrupt(digitalPinToInterrupt(7));
  pin_interrupt_flag = 1;

  
}
*/

static long timeSleep = 0;  // total time due to sleep
static float calibv = 0.93; // ratio of real clock with WDT clock
static volatile uint8_t isrcalled = 0;  // WDT vector flag

// Internal function: Start watchdog timer
// byte psVal - Prescale mask
static void WDT_On (uint8_t psVal)
{
 // prepare timed sequence first
 uint8_t ps = (psVal | (1<<WDIE)) & ~(1<<WDE);
 cli();
 wdt_reset();
 // Clear WDRF in MCUSR 
 MCUSR &= ~(1<<WDRF);
 // start timed sequence
 WDTCSR |= (1<<WDCE) | (1<<WDE);
 // set new watchdog timeout value
 WDTCSR = ps;
 sei();
}

// Internal function.  Stop watchdog timer
static void WDT_Off() {
 cli();
 wdt_reset();
 // Clear WDRF in MCUSR 
 MCUSR &= ~(1<<WDRF);
 // Write logical one to WDCE and WDE 
 // Keep old prescaler setting to prevent unintentional time-out
 WDTCSR |= (1<<WDCE) | (1<<WDE);
 // Turn off WDT 
 WDTCSR = 0x00;
 sei();
}

// internal function.  
static int doSleep(long timeRem) {
 uint8_t WDTps = 9;  // WDT Prescaler value, 9 = 8192ms

 isrcalled = 0;
 sleep_enable();
 while(timeRem > 0) {
   //work out next prescale unit to use
   while ((0x10<<WDTps) > timeRem && WDTps > 0) {
     WDTps--;
   }
   // send prescaler mask to WDT_On
   WDT_On((WDTps & 0x08 ? (1<<WDP3) : 0x00) | (WDTps & 0x07));
   isrcalled=0;
   while (isrcalled==0) {
     // turn bod off
      // MCUCR |= (1<<BODS) | (1<<BODSE);
     //MCUCR &= ~(1<<BODSE);  // must be done right before sleep
     sleep_cpu();  // sleep here
   }
   // calculate remaining time
   timeRem -= (0x10<<WDTps);
 }
 sleep_disable();
 return timeRem;
}

// Calibrate watchdog timer with millis() timer(timer0)
static void calibrate() {
 // timer0 continues to run in idle sleep mode
 set_sleep_mode(SLEEP_MODE_IDLE);
 long tt1=millis();
 doSleep(256);
 long tt2=millis();
 calibv = 256.0/(tt2-tt1);
}

// Estimated millis is real clock + calibrated sleep time
static long estMillis() {
 return millis()+timeSleep;
}



// Delay function
static void sleepCPU_delay(long sleepTime) {
 //ADCSRA &= ~(1<<ADEN);  // adc off
 //PRR = 0xEF; // modules off

 set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 int trem = doSleep(sleepTime*calibv);
 timeSleep += (sleepTime-trem);

 //PRR = 0x00; //modules on
 //ADCSRA |= (1<<ADEN);  // adc on
}



// wdt int service routine
ISR(WDT_vect) {
 WDT_Off();
 isrcalled=1;
}

//ISR(PCINT0_vect){
//  LORA_LED_PORT ^= LORA_LED_BIT;  
//}


/*
static void sleep_watchdog_blinky(void){  

  RGB_BLUE_PORT |= RGB_BLUE_BIT;
  RGB_BLUE_DDR  |= RGB_BLUE_BIT;

  LORA_LED_PORT |= LORA_LED_BIT;
  LORA_LED_DDR  |= LORA_LED_BIT;

  //calibrate();

//3.1: PINE6 PCINT6 arduino pin 7
//3.0: PINB7 PCINT7 arduino pin 11

  //pinMode(7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), KISSLoRa_led_toggle, LOW);//FALLING
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  power_usb_disable();  
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();
  //output compare modulator?
  power_adc_disable();
  power_usart0_disable();
  power_usart1_disable();
  power_spi_disable();
  power_twi_disable();

  
  while(1){
     sleep_cpu(); 

    if(pin_interrupt_flag){
      pin_interrupt_flag = 0;
      LORA_LED_PORT ^= LORA_LED_BIT; 

       while((BUTTON_IN_PIN & BUTTON_IN_BIT)==0);//wait for button to be released
       attachInterrupt(digitalPinToInterrupt(7), KISSLoRa_led_toggle, LOW);//FALLING
    }


     
    
    //sleepCPU_delay(5000-25);
    //RGB_BLUE_PORT ^=  RGB_BLUE_BIT;
    //sleepCPU_delay(25);
   // RGB_BLUE_PORT ^=  RGB_BLUE_BIT;
  }
}
*/

/*
void sleep_test(void){
  //wake_blinky();
  //sleep_blinky();
  sleep_watchdog_blinky();
}
*/

//! \brief initializes sleep mode and calibrates watchdog timer
void KISSLoRa_sleep_init(void){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  calibrate();
  
/*
  power_usb_disable();  
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();
  //output compare modulator?
  power_adc_disable();
  power_usart0_disable();
  power_usart1_disable();
  power_spi_disable();
  power_twi_disable();
  */
}

//! \brief powers down peripherals and puts microcontroller in power down sleep mode, wakes up on watchdog timer
void KISSLoRa_sleep_delay_ms(long delay_ms){
  power_usb_disable();  
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();
  //output compare modulator?
  power_adc_disable();
  power_usart0_disable();
  //power_usart1_disable();//lora
  power_spi_disable();
  power_twi_disable();
  
  sleepCPU_delay(delay_ms);

  power_usb_enable();  
  power_timer0_enable();
  power_timer1_enable();
  power_timer2_enable();
  power_timer3_enable();
  //output compare modulator?
  power_adc_enable();
  power_usart0_enable();
  //power_usart1_enable();//lora
  power_spi_enable();
  power_twi_enable();
}

