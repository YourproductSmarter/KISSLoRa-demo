/*
  KISSLoRa-demo_app.ino - demo application for the KISS LoRa gadget

  The MIT License (MIT)

  Copyright (c) 2017 Gepro|Electronics  www.yourproductsmarter.com

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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <Arduino.h>
#include <TheThingsNetwork.h>
#include "I2C.h"
#include "LoraMessage.h"
#include "LoraEncoder.h"
#include "SoftPWM.h"
#include "KISSLoRa_sleep.h"
#include "KISSLoRa_io.h"

//Define both serial interfaces for easier use
#define loraSerial			Serial1
#define usbserial				Serial//for commisioning and debugging

//SI7021 defines
#define SI7021_ADDRESS			0X40	//I2C slave address
#define SI7021_HUMIDITY			0XF5	//Measure Relative Humidity, No Hold Master Mode
#define SI7021_PREV_TEMPERATURE	0XE0	//Read Temperature Value from Previous RH Measurement

//FXLS8471Q defines
#define FXLS8471Q_ADDRESS		   0x1D	//I2C slave address
#define FXLS8471Q_WHOAMI		   0x0D	//Address of the WHO_AM_I register
#define FXLS8471Q_CTRL_REG1		 0x2A	//Address of the CRTL_REG1 register
#define FXLS8471Q_OUT_X_MSB		 0x01	//Address of the OUT_X_MSB register
#define FXLS8471Q_OUT_X_LSB    0x02  
#define FXLS8471Q_OUT_Y_MSB		 0x03	//Address of the OUT_Y_MSB register
#define FXLS8471Q_OUT_Y_LSB    0x04
#define FXLS8471Q_OUT_Z_MSB		 0x05	//Address of the OUT_Z_MSB register
#define FXLS8471Q_OUT_Z_LSB    0x06
#define FXLS8471Q_XYZ_DATA_CFG 0x0E //Address of the XYZ_DATA_CFG register

//Mode/state defines
#define UNKNOWN_MODE        0
#define COMMISSIONING_MODE  1
#define APPS_STATE          2

//TheThingsNetwork anonymous config
static const char *devAddr = "26011F0F";
static const char *nwkSKey = "ADAFF80790E39125AD12F170768A6A97";
static const char *appSKey = "B98AEF49D4D819536FDD511544FCC49B";

#define freqPlan TTN_FP_EU868

typedef struct
{
	int8_t acc_x = 0;
	int8_t acc_y = 0;
	int8_t acc_z = 0;
}FXLS8471Q_values;	//Struct to save position (x,y,z) variables

//Function prototypes
static void init_FXLS8471Q(void);
static void get_acc_values(FXLS8471Q_values *pacc_values);
static uint16_t get_si7021_value(uint8_t command);
static uint16_t get_lux_value();
static uint8_t get_rotary_value();
static void set_lora_led(bool state);
static void set_rgb_led(uint8_t R, uint8_t G, uint8_t B);
static void sleep(uint32_t delay_time_ms, uint8_t rotary_value);

//Global variables
TheThingsNetwork ttn(loraSerial, usbserial, freqPlan); //Init TTN class
uint8_t humidity = 0;			//Humidity value in percent
float temperature = 0;		//Temperature value in degrees Celsius
uint16_t lux = 0;				//Light intensity in lux
FXLS8471Q_values acc_values;	//Accelerometer x,y,z positions
uint8_t current_state = 1;
double time_to_sleep = 1;
uint8_t joined_network = 0;
uint16_t hwEui_16_bits = 0;
char hwEui_char_array[16+1];//16 chars + \0
bool connectivity_check;
uint8_t app, prev_app;
unsigned long prev_message_at_millis = 0;

int main(void)
{
	init();						//Init Arduino

  KISSLoRa_sleep_init();
  
	//Attach USB. Necessary for the serial monitor to work
	#if defined(USBCON)
		USBDevice.attach();
    USBCON|=(1<<OTGPADE); //enables VBUS pad for detection of USB cable
	#endif
  
	SoftPWMBegin();

  //set leds as output:
  LORA_LED_DDR  |= LORA_LED_BIT;
  RGB_RED_DDR   |= RGB_RED_BIT;
  RGB_BLUE_DDR  |= RGB_BLUE_BIT;
  RGB_GREEN_DDR |= RGB_GREEN_BIT;
  
  //turn leds off(active low):
  LORA_LED_PORT  |= LORA_LED_BIT;
  RGB_RED_PORT   |= RGB_RED_BIT;
  RGB_BLUE_PORT  |= RGB_BLUE_BIT;
  RGB_GREEN_PORT |= RGB_GREEN_BIT;

  //init red and green LED's as SoftPWM
  SoftPWMSetPolarity(12,SOFTPWM_INVERTED);
  SoftPWMSetFadeTime(12, 0, 1);
  SoftPWMSetPolarity(6,SOFTPWM_INVERTED);
  SoftPWMSetFadeTime(6, 1, 1);

  //set the analog reference to the internal reference
  analogReference(INTERNAL);
  
  //Start serial ports
	loraSerial.begin(57600);	//RN2483 needs 57600 baud rate  
	usbserial.begin(9600);


  //Wait a maximum of 10s for serial to receive data, if no cable is connected don't wait
	while (!usbserial.available() && millis() < 10000 && USB_CABLE_CONNECTED) {
    set_rgb_led(0,0,255);
	  delay(250);
    set_rgb_led(0,0,0);
    delay(250);
	}

  //if no data was received or no usb cable is connected, stop usbserial
  if(!usbserial.available() || !USB_CABLE_CONNECTED){
    usbserial.end();
  }
 
  usbserial.println(F("--- Resetting RN module"));
  ttn.reset(false);
  
  usbserial.println(F("--- Random Seed"));
  ttn.getHardwareEui(hwEui_char_array, 17);                                    //read HWEUI from module
  usbserial.print(F("-- hwEui read from module: "));
  usbserial.println(hwEui_char_array);
  hwEui_16_bits = ascii_hex_to_nibble(hwEui_char_array[12]);
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[13]) << 4;
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[14]) << 8;
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[15]) << 12;
  uint16_t seed = hwEui_16_bits;
  usbserial.print("-- seed: ");
  usbserial.println(seed);
  randomSeed(seed);
	I2c.begin();				  //Init I2C peripheral
	I2c.timeOut(1000);			//Set I2C timeout to 1 second
	init_FXLS8471Q();			//Init accelerometer

  //Always start with Unknown mode (8sec)
  current_state = UNKNOWN_MODE;

  String serialDataIn;

  //enter commissioning mode when serial data from usb is available and stay until commissioning is done(join succesful) or terminated(usb cable removed)
  while(usbserial.available() || current_state == COMMISSIONING_MODE) 
  {
  
    current_state = COMMISSIONING_MODE;
    
    serialDataIn = usbserial.readStringUntil('\n');                     //read incoming data
    if(serialDataIn.startsWith("getdeveui"))                            //gets DEV eui
    {
      char hwEui[16+1];                                                 //16 chars + \0
      ttn.getHardwareEui(hwEui, sizeof(hwEui));                         //read HWEUI from module
      usbserial.print("deveui:" + (String)hwEui + '\n');
    }
    if(serialDataIn.startsWith("setappeui:"))                           //sets new app eui AND APP KEY in LoRa module
    {
      String newAppEuiString = serialDataIn.substring(10,26);
      String newAppKeyString = serialDataIn.substring(37,69);
      char const*newAppEui = newAppEuiString.c_str();
      char const*newAppKey = newAppKeyString.c_str();
      //program new appeui and appkey in LoRa module
      if (ttn.provision(newAppEui, newAppKey)) {
        usbserial.print("appeuiset:" + (String)newAppEui + ",appkeyset:" + (String)newAppKey + "\n");
      } else {
        usbserial.print(F("error:invalid keys\n"));
      }
    }
    if(serialDataIn.startsWith("setrgb:"))                              //sets colour of RGB led
    {
      String redString = serialDataIn.substring(7,9);                   //get colour values (HEX)
      String greenString = serialDataIn.substring(9,11);
      String blueString = serialDataIn.substring(11,13);
      int redInt = (int)strtol(&redString[0],NULL,16);                  //convert and invert to integer value 0-255 (DEC)
      int greenInt = (int)strtol(&greenString[0],NULL,16);
      int blueInt = (int)strtol(&blueString[0],NULL,16);
      set_rgb_led(redInt,greenInt,blueInt); 
      usbserial.print("rgbset:");
      usbserial.print(redString);
      usbserial.print(greenString);
      usbserial.print(blueString);
      usbserial.print("\n");
    }
    if(serialDataIn.startsWith("rejoin"))                               //rejoins LoRa network with new keys (i.e. reboot)
    {
      joined_network = ttn.join();         //Join TTN network and show progress on via serial
      if (joined_network) {
        usbserial.print("ack\n");
        current_state = APPS_STATE;
      } else {
        usbserial.print(F("error:join failed\n"));
      }
      //goto PERSONAL_MODE
    }
    if(serialDataIn.startsWith("debug"))
    {
      ttn.showStatus();
    }
    //if no USB cable is connected, stop commissioning mode
    if(!USB_CABLE_CONNECTED)  
    {
      if(usbserial)
        usbserial.end();
      current_state = APPS_STATE;
    }
  }//end commissioning mode

  
  //if no network is joined, join network
  if (!joined_network) {
    char appEui[17];
    ttn.getAppEui(appEui, sizeof(appEui));
    if (strcmp(appEui, "0000000000000000") == 0) {
      ttn.personalize(devAddr, nwkSKey, appSKey);
    } else {
      ttn.join();
    }
    joined_network = 1;
  }
  
  //Start LoRa communication
	usbserial.println(F("-- Status"));
	ttn.showStatus();			//Show RN2483 information via serial
 
	while (1)
	{
    //if no USB cable is connected, stop usbserial
    if(!USB_CABLE_CONNECTED)
    {
      if(usbserial)
        usbserial.end();
    }
    
    //determine app selected with rotary switch
    app = get_rotary_value();
    usbserial.print(F("Selected app: "));
    usbserial.println(app);
    switch(app)
    {
      case 0://app 0: send lora messages and sleep for xx seconds        
        {
          set_rgb_led(0, 0, 0);
          read_sensors_send_lora();    
          sleep(20*1000, 0);
        }
        break;//app 0
      case 1://app 1: countdown to next LoRa message (fade blue LED to off, then send message)
        {
          set_rgb_led(0, 0, 0);
          uint16_t count = 200;
          uint8_t intensity;
          //read sensors and send lora message
          read_sensors_send_lora();
          while(count-- && get_rotary_value() == 1)
          {
            //determine intensity
            intensity = map(count, 0, 200, 1, 255);
            //write output
            set_rgb_led(0,0,intensity);
            delay(100);  
            if( (BUTTON_IN_PIN & BUTTON_IN_BIT) == 0 )
            {
              connectivity_check = true;
              break;
            }
          }
        }
        break;//app 1
      case 2://app 2: LoRa connectivity check. Checks when button is pressed.
        {
          set_rgb_led(0, 0, 0);
          //if button is pressed or button was pressed while in sleep
          if( (BUTTON_IN_PIN & BUTTON_IN_BIT) == 0 || connectivity_check == true)
          {
            connectivity_check = false;
            //send message
            set_lora_led(true);
            joined_network = ttn.poll(1, true); 
            set_lora_led(false);
            //check if we received an acknowledge
            if(joined_network == TTN_SUCCESSFUL_RECEIVE)
            {
              //blink green led
              for(int i = 0; i < 6; i++)
              {
                set_rgb_led(0,255,0);
                delay(250);
                set_rgb_led(0,0,0);
                delay(250);
              }  
            }
            else
            {
              //blink red led
              for(int i = 0; i < 6; i++)
              {
                set_rgb_led(255,0,0);
                delay(250);
                set_rgb_led(0,0,0);
                delay(250);
              }  
            }
          }
          //sleep for 60 seconds
          sleep(60U*1000, 2);
        }
        break;//app 2
      case 3://app 3: accelerometer -> RGB colourwheel
        {
          int16_t intensity;
          int16_t angle;
          //read acc
          get_acc_values(&acc_values);//x and y are used to make a vector: length = intensity, angle = color
          //determine intensity
          intensity = (acc_values.acc_x*acc_values.acc_x + acc_values.acc_y*acc_values.acc_y);//use Pythagorean theorem to determine length
          intensity = map(intensity, 0, 8192, 1, 255);
          if(intensity < 1)
            intensity = 1;
          //calculate angle
          angle = (180.0/PI) * atan(((float)(acc_values.acc_x) / (float)(acc_values.acc_y))); 
          if(acc_values.acc_y >= 0)
          {//angle between 270 and 360
            if(acc_values.acc_x <= 0)
            {
              angle += 360;
            }
          }
          else
          {//angle between 180 and 270
            angle += 180;
          }
          
          //determine output colour:
          //  -\__/- Red
          //  /--\_- Green
          //  __/--\ Blue 
          if(angle >= 300)
          {
            set_rgb_led(intensity,1,map(angle, 300, 360, intensity, 1));
          }
          else if(angle >= 240)
          {
            set_rgb_led(map(angle, 240, 300, 1, intensity),1,intensity);
          }
          else if(angle >= 180)
          {
            set_rgb_led(1,map(angle, 180, 240, intensity, 1),intensity);
          }
          else if(angle >= 120)
          {
            set_rgb_led(1,intensity,map(angle, 120, 180, 1, intensity));
          }
          else if(angle >= 60)
          {
            set_rgb_led(map(angle, 60, 120, intensity, 1),intensity,1);
          }
          else
          {
            set_rgb_led(intensity,map(angle, 0, 60, 1, intensity),1);
          }
        }
        break;//app 3
      case 4://app 4: accelerometer -> RGB: >  RED = x > 2g, GREEN = y > 2g, BLUE = z > 2g
        {
          uint8_t R, G, B;
          //read acc sensor
          get_acc_values(&acc_values);
          //determine outputs
          if(acc_values.acc_x >= 80 || acc_values.acc_x <= -80)
            R = 255;
          else
            R = 0;
          if(acc_values.acc_y >= 80 || acc_values.acc_y <= -80)
            G = 255;
          else
            G = 0;
          if(acc_values.acc_z >= 80 || acc_values.acc_z <= -80)
            B = 255;
          else
            B = 0;
          //write outputs
          set_rgb_led(R,G,B);
          delay(100);
        }
        break;//app 4
      case 5://app 5: accelerometer dice
        {
          uint8_t count;
          uint8_t R, G, B;
          uint8_t randomnumber;
          //first time so init
          if(app != prev_app)
          {
            set_rgb_led(0,0,0);
            //init random number generator
            get_random_seed();          
          }
          //read acc sensor
          get_acc_values(&acc_values);
          //if any G force is > 3G, start "rolling" the dice
          if(acc_values.acc_x > 120 || acc_values.acc_y > 120 || acc_values.acc_z > 120
          || acc_values.acc_x < -120 || acc_values.acc_y < -120 || acc_values.acc_z < -120)
          {
            count = 30;
            
            do//use do while loop to go through loop at least once to get a the number and result, exit after 30 cycles or after switch has changed
            {
              randomnumber = random(1, 7);
              switch(randomnumber)
              {
                case 1:
                  R = 255;
                  G = 0;
                  B = 0;
                break;
                case 2:
                  R = 255;
                  G = 255;
                  B = 0;
                break;
                case 3:
                  R = 0;
                  G = 255;
                  B = 0;
                break;
                case 4:
                  R = 0;
                  G = 255;
                  B = 255;
                break;
                case 5:
                  R = 0;
                  G = 0;
                  B = 255;
                break;
                case 6:
                  R = 255;
                  G = 0;
                  B = 255;
                break;
                default:
                  R = 0;
                  G = 0;
                  B = 0;
                break; 
              }
              //blink output
              set_rgb_led(R, G, B);
              delay(40);
            }
            while(--count && (get_rotary_value() == 5));
            
            set_rgb_led(0, 0, 0);
            delay(1000);
            for(count = 0; count <= randomnumber; count++)
            {
              set_rgb_led(R, G, B);
              delay(250);
              set_rgb_led(0, 0, 0);
              delay(250);              
            }
          }
        }
        break;//app 5
      case 6://app 6: RGB fade
        {
          uint16_t R, G, B;
          uint8_t FADESPEED = 5;
          //if we just got to this app, turn LED's off and start fading to blue
          if(app != prev_app)
          {
            set_rgb_led(0, 0, 0);
            for (B = 0; B < 256; B++) { 
              set_rgb_led(R, G, B);
              delay(FADESPEED);
              if(get_rotary_value() != 6)
                break;
            }
            B = 255;
          }  
          // fade from blue to violet
          for (R = 1; R < 256; R++) { 
            set_rgb_led(R, G, B);
            delay(FADESPEED);
            if(get_rotary_value() != 6)
              break;
          }
          R = 255; 
          // fade from violet to red
          for (B = 255; B > 0; B--) { 
            set_rgb_led(R, G, B);
            delay(FADESPEED);
            if(get_rotary_value() != 6)
              break;
          } 
          B = 0;
          // fade from red to yellow
          for (G = 1; G < 256; G++) { 
            set_rgb_led(R, G, B);
            delay(FADESPEED);
            if(get_rotary_value() != 6)
              break;
          } 
          G = 255;
          // fade from yellow to green
          for (R = 255; R > 1; R--) { 
            set_rgb_led(R, G, B);
            delay(FADESPEED);
            if(get_rotary_value() != 6)
              break;
          } 
          R = 1;
          // fade from green to teal
          for (B = 0; B < 256; B++) { 
            set_rgb_led(R, G, B);
            delay(FADESPEED);
            if(get_rotary_value() != 6)
              break;
          } 
          B = 255;
          // fade from teal to blue
          for (G = 255; G > 1; G--) { 
            set_rgb_led(R, G, B);
            delay(FADESPEED);
            if(get_rotary_value() != 6)
              break;
          }
          G = 1;
        } 
        break;  //app 6
      case 7://app 7: Light sensor -> RGB (White) LED
        {
          int16_t intensity;
          //get light sensor value
          lux = get_lux_value();
          //determine intensity
          intensity = map(lux, 0, 250, 255, 1);
          if(intensity < 1)
            intensity = 1;
          //set output
          set_rgb_led(intensity,intensity,intensity);
          delay(10);
        }
        break;//app 7
      case 8://app 8: temperature sensor -> RGB (cold = blue, normal is green, hot is red)
        {
          //read humidity sensor (otherwise temperature sensor doesn't work)
          /*uint16_t rh_code = */get_si7021_value(SI7021_HUMIDITY);
          //read temperature sensor
          uint16_t temp_code = get_si7021_value(SI7021_PREV_TEMPERATURE);
          //calculate temperature
          if (temp_code == 100) temp_code = 0;      //Check for error
          temperature = (175.25*temp_code/65536)-46.85; //Convert raw data to readable value
          //determine output
          if(temperature <= 18)
            set_rgb_led(0, 0, 255);
          else if(temperature <= 24)
            set_rgb_led(0, 255, 0);
          else 
            set_rgb_led(255, 0, 0);
          delay(500);
        }
        break;//app 8
      case 9://app 9: Humidity sensor -> blink blue LED (higher humidity is faster blinking)
        {
          //read humidity sensor
          uint16_t rh_code = get_si7021_value(SI7021_HUMIDITY);
          //uint16_t temp_code = get_si7021_value(SI7021_PREV_TEMPERATURE);

          uint16_t delaytime;
          //calculate humidity
          if (rh_code == 100) rh_code = 0;        //Check for error
          humidity = (125.0*rh_code/65536)-6;       //Convert raw data to readable value
          //determine delay time
          delaytime = map(humidity, 0, 100, 0, 1980);
          if(delaytime > 1980) delaytime = 1980;
          //blink led
          set_rgb_led(0, 0, 255);
          delay(2000-delaytime);
          set_rgb_led(0, 0, 0);
          delay(2000-delaytime);
        }
        break;//app 9
      default:
          //should not occur, do nothing
        break;
    }
    prev_app = app;
    //if app is not an app where LoRa messages are sent, send a message anyway
    if(app >= 2 && app <= 9)
    {
      if(millis() > (prev_message_at_millis + 20000))//send message every 20 seconds
      {
        prev_message_at_millis = millis();
        read_sensors_send_lora();
      }    
    }
	}
//  usbserial.println("Shouldn't be here...");
	return 0;
}

//! \brief read sensors and send lora message
static void read_sensors_send_lora(void)
{
  //Collect all sensor data state and control i/o
  usbserial.println(F("-- Collecting sensordata"));
  uint16_t rh_code = get_si7021_value(SI7021_HUMIDITY);
  if (rh_code == 100) rh_code = 0;        //Check for error
  humidity = (125.0*rh_code/65536)-6;       //Convert raw data to readable value
  usbserial.print(F("-- Humidity value converted from I2C value: "));
  usbserial.println(humidity);
  
  uint16_t temp_code = get_si7021_value(SI7021_PREV_TEMPERATURE);
  if (temp_code == 100) temp_code = 0;      //Check for error
  temperature = (175.25*temp_code/65536)-46.85; //Convert raw data to readable value
  usbserial.print(F("-- Temperature value converted from I2C value: "));
  usbserial.println(temperature);
  
  lux = get_lux_value();
  usbserial.print(F("-- Lux value converted from analog: "));
  usbserial.println(lux);
  
  get_acc_values(&acc_values);
  
  ttn.getHardwareEui(hwEui_char_array, 17);                                    //read HWEUI from module
  usbserial.print(F("-- hwEui read from module: "));
  usbserial.println(hwEui_char_array);
  hwEui_16_bits = ascii_hex_to_nibble(hwEui_char_array[12]);
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[13]) << 4;
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[14]) << 8;
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[15]) << 12;

  //Decode and send data state (only if there is a LoRa network available)
  set_lora_led(true);
  LoraMessage message;
  
  //Add all data to the message class which decodes it
  usbserial.println(F("-- Decoding following data: "));
  usbserial.print(F("-- Humidity: "));
  usbserial.println(humidity);
  message.addUint8(humidity);
  
  usbserial.print(F("-- Temperature: "));
  usbserial.println(temperature);
  message.addTemperature(temperature);
  
  usbserial.print(F("-- Lux: "));
  usbserial.println(lux);
  message.addUint16(lux);
  
  usbserial.print(F("-- Accelerometer x position: "));
  usbserial.println(acc_values.acc_x);
  message.addUint8(acc_values.acc_x);
  
  usbserial.print(F("-- Accelerometer y position: "));
  usbserial.println(acc_values.acc_y);
  message.addUint8(acc_values.acc_y);
  
  usbserial.print(F("-- Accelerometer z position: "));
  usbserial.println(acc_values.acc_z);
  message.addUint8(acc_values.acc_z);
  
  usbserial.print(F("-- hwEui: "));
  usbserial.println(hwEui_16_bits, HEX);
  message.addUint16(hwEui_16_bits);
  
  //Send the decoded data
  
  uint8_t txsf = 7;
  uint8_t rnd = (uint8_t)random(1, 64);
  
  if (rnd == 1 ) {
    txsf = 12;  // SF12
  }
  else if (rnd > 1  && rnd < 4  ) {
    txsf = 11;  // SF11
  }
  else if (rnd > 3  && rnd < 8  ) {
    txsf = 10;  // SF10
  }
  else if (rnd > 7  && rnd < 16 ) {
    txsf = 9;  // SF9
  }
  else if (rnd > 15 && rnd < 32 ) {
    txsf = 8;  // SF8
  }
  else {
    txsf = 7;  // SF7
  }
  
  usbserial.println(F("-- Setting Data Rate"));
  usbserial.print(F("--- SF: "));
  usbserial.println(txsf);
    
  usbserial.println(F("-- Sending data"));
  joined_network = ttn.sendBytes(message.getBytes(), message.getLength(), 1, connectivity_check, txsf); 
  connectivity_check = false;
  if(connectivity_check == true)
  {
    if(joined_network == TTN_SUCCESSFUL_RECEIVE)
    {
      //blink green led
      for(int i = 0; i < 6; i++)
      {
        set_rgb_led(0,255,0);
        _delay_ms(250);
        set_rgb_led(0,0,0);
        _delay_ms(250);
      }  
    }
    else
    {
      //blink red led
      for(int i = 0; i < 6; i++)
      {
        set_rgb_led(255,0,0);
        delay(250);
        set_rgb_led(0,0,0);
        delay(250);
      }  
    }
  }
  set_lora_led(false);
}
//

//! \brief Configure and activate the FXLS8471Q Accelerometer 
static void init_FXLS8471Q(void)
{
	uint16_t who = 0;
	
	//Check if the chip responds to the who-am-i command, should return 0x6A (106)
	I2c.read(FXLS8471Q_ADDRESS, FXLS8471Q_WHOAMI, 1);
	who = I2c.receive();
	if (who == 106)
	{
		//Configure FXLS8471Q CTRL_REG1 register
		//Set f_read bit to activate fast read mode
		//Set active bit to put accelerometer in active mode
		I2c.write(FXLS8471Q_ADDRESS, FXLS8471Q_CTRL_REG1, 0x03);
		usbserial.println(F("--- I2C Accelerometer initialized"));
		
	}else{
		usbserial.println(F("--- I2C Accelerometer not initialized"));
	}	
}

//! \brief set the range of the FXLS8471Q Accelerometer, valid ranges: 2G,4G or 8G
static void set_acc_range(uint8_t range_g)
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
  I2c.write((uint8_t)FXLS8471Q_ADDRESS, (uint8_t)FXLS8471Q_XYZ_DATA_CFG, range_b);//typecast to get rid of warning
}

//! \brief Read Accelerometer values to a given variable
static void get_acc_values(FXLS8471Q_values *pacc_values)
{
	usbserial.print(F("--- I2C Selecting FXLS8471Q slave with address: "));
	usbserial.println(FXLS8471Q_ADDRESS, HEX);
	
	//read x value
	usbserial.print(F("--- I2C Reading from register address: "));
	usbserial.println(FXLS8471Q_OUT_X_MSB, HEX);
	I2c.read(FXLS8471Q_ADDRESS,FXLS8471Q_OUT_X_MSB,1);	
	pacc_values->acc_x = I2c.receive();
	usbserial.print(F("--- I2C Returned value x: "));	
	usbserial.println(pacc_values->acc_x);
	
	//read y value
	usbserial.print(F("--- I2C Reading from register address: "));
	usbserial.println(FXLS8471Q_OUT_Y_MSB, HEX);
	I2c.read(FXLS8471Q_ADDRESS,FXLS8471Q_OUT_Y_MSB,1);
	pacc_values->acc_y = I2c.receive();
	usbserial.print(F("--- I2C Returned value y: "));
	usbserial.println(pacc_values->acc_y);
	
	//read z value
	usbserial.print(F("--- I2C Reading from register address: "));
	usbserial.println(FXLS8471Q_OUT_Z_MSB, HEX);
	I2c.read(FXLS8471Q_ADDRESS,FXLS8471Q_OUT_Z_MSB,1);
	pacc_values->acc_z = I2c.receive();
	usbserial.print(F("--- I2C Returned value z: "));
	usbserial.println(pacc_values->acc_z);
}

//! \brief get random seed for dice app, uses lux sensor and accelerometer to generate 32-bit random seed
static void get_random_seed(void){
 
  //init random number generator

  uint8_t rand_lux;
  uint8_t rand_x;
  uint8_t rand_y;
  uint8_t rand_z;

  rand_lux = get_lux_value();
  
  set_acc_range(0);

  I2c.write(FXLS8471Q_ADDRESS, FXLS8471Q_CTRL_REG1, 0x01);//set to 14 bit read

  I2c.read(FXLS8471Q_ADDRESS,FXLS8471Q_OUT_X_MSB,2);  
  I2c.receive();
  rand_x = I2c.receive();

  I2c.read(FXLS8471Q_ADDRESS,FXLS8471Q_OUT_Y_MSB,2); 
  I2c.receive(); 
  rand_y = I2c.receive();

  I2c.read(FXLS8471Q_ADDRESS,FXLS8471Q_OUT_Z_MSB,2);  
  I2c.receive();
  rand_z = I2c.receive();

  I2c.write(FXLS8471Q_ADDRESS, FXLS8471Q_CTRL_REG1, 0x03);//return settings to normal(8-bit read)

  uint32_t seed_value = rand_lux*(1UL) + rand_x*(256UL) + rand_y*(256UL*256)+ rand_z*(256UL*256*256);//combine light sensor and accelero sensor to generate random seed
  randomSeed(seed_value);
}

//! \brief Use a I2C command to get a value from the SI7021 Humidity and Temperature sensor
//!	Returns 100 if the I2C bus didn't return any data
static uint16_t get_si7021_value(int command)
{
	uint16_t nBytes = 3;
	//if we are only reading old temperature, read only msb and lsb
	if (command == 0xE0) nBytes = 2;
	
	
	usbserial.print(F("--- I2C Selecting SI7021 slave with address: "));
	usbserial.println(SI7021_ADDRESS, HEX);
	usbserial.print(F("--- I2C Sending command code: "));
	usbserial.println(command, HEX);

	I2c.write(SI7021_ADDRESS, command);
	delay(50); //Wait for measurement. Datasheet suggest at least 22ms
	I2c.read(SI7021_ADDRESS, nBytes);

	if(I2c.available())
	{
		uint8_t msb = I2c.receive();
		uint8_t lsb = I2c.receive();
		// Clear the last to bits of LSB to 00.
		// According to datasheet LSB of RH is always xxxxxx10
		lsb &= 0xFC;
		uint16_t measurement = msb << 8 | lsb;
		usbserial.print(F("--- I2C returned value: "));
		usbserial.println(measurement);
		return measurement;
	}
	//If there is no I2C data available return following value as error code
	usbserial.println(F("--- I2C error 100: SI7021 didn't return a I2C value.\n"));
	return 100;
}

//! \brief Return lux value from the APDS-9007 Ambient Light Photo Sensor
static uint16_t get_lux_value()        //to be repaired. measuring but value not accurate
{
  //! \todo find out if changing double to float saves flash or time
	double digital_value = analogRead(10);
	usbserial.print(F("--- AnalogRead returned: "));
	usbserial.println(digital_value);
  double vlux = digital_value * (2.56/1023.0);//lux value in volts
	double ilux = (vlux / 56) * 1000;	//lux value in micro amperes
	double lux = pow(10, (ilux / 10));		//Convert ilux to Lux value
	return (uint16_t)lux;					//Return Lux value as value without decimal
}

//! \brief Return current value (0-9) from the rotary switch.
static uint8_t get_rotary_value()
{
	uint8_t value = 0;

  //"If PORTxn is written logic one when the pin is configured as an input pin, the pull-up resistor is activated. To
  //switch the pull-up resistor off, PORTxn has to be written logic zero or the pin has to be configured as an output pin."

  //re-init rotary switch input
  ROTARY_8_DDR &= ~ROTARY_8_BIT; //set as inputs
  ROTARY_8_PORT &= ~ROTARY_8_BIT;//disable pull-ups, because external pull-down resistors are used
  
	//Read each rotary switch i/o line and change value if a line is high
	if (ROTARY_1_PIN & ROTARY_1_BIT) {value |= 1 << 0;}
	if (ROTARY_2_PIN & ROTARY_2_BIT) {value |= 1 << 1;}
	if (ROTARY_4_PIN & ROTARY_4_BIT) {value |= 1 << 2;}
  if (ROTARY_8_PIN & ROTARY_8_BIT) {value |= 1 << 3;}

	//Set value to 0 if it's higer than 9 (this should never happen but just in case)
	if (value > 9)
	{
		value = 0;
	}
	return value;
}

//! \brief Turn LoRa led on (true) or off (false)
static void set_lora_led(bool state)
{
	if (!state)
	{
		LORA_LED_PORT |= LORA_LED_BIT; //Turn led off
	}
	else
	{
		LORA_LED_PORT &= ~LORA_LED_BIT; //Turn led on
	}
}

//! \brief Turn RGB led on with a given state colour of turn it off
static void set_rgb_led(uint8_t R, uint8_t G, uint8_t B)
{
  if(R == 0){  
    SoftPWMEnd(12);
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH);
  }
  else{
    SoftPWMSet(12,255-R);
  }
  if(G == 0){  
    SoftPWMEnd(6);
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
  }
  else{
    SoftPWMSet(6,255-G);
  }
    
  #ifdef REVISION_3_1
    analogWrite(11, 255-B);
  #else
    if(B == 255)
      digitalWrite(4, HIGH);
    else
      digitalWrite(4, LOW);
  #endif
}

//! \brief Sleep until a given time has passed, or if the push button is pressed, or if rotary switch is changed
static void sleep(uint32_t delay_time_ms, uint8_t rotary_value)
{
 
  //Loop until delay is over, or if the push button is pressed, or if rotary switch is changed
  while (delay_time_ms)
  {
    if(!USB_CABLE_CONNECTED)
    {
      KISSLoRa_sleep_delay_ms(100);
    }
    else
    {
      delay(100);
    }

    if(delay_time_ms > 100){
      delay_time_ms -= 100;
    }
    else{
      delay_time_ms = 0;
    }
    
    //Check if button is pressed
    if( (BUTTON_IN_PIN & BUTTON_IN_BIT) == 0 )
    {
      connectivity_check = true;
      break;
    }
    if(get_rotary_value() != rotary_value)
    {
      break;      
    }
  }
}

//! \brief This function is used to convert ascii-hex string to integer
static uint8_t ascii_hex_to_nibble(char ascii_hex)
{  
  uint8_t return_value=0;

  if((ascii_hex >= 'A') && (ascii_hex <= 'F'))
  {
    return_value |= (ascii_hex - ('A' - 10));
  }
  else if((ascii_hex >= '0') && (ascii_hex <= '9'))
  {
    return_value |= (ascii_hex - '0');
  }
  return return_value;
}

