/*
  PushButton.ino - An example for the KISS LoRa device
  
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

#define BUTTON_PIN 7
//#define BUTTON_PIN 11 //on a previous version of the board the button was on this pin

#define LED_PIN 12

void setup() {
  //Set pin as inputs
  pinMode(BUTTON_PIN, INPUT);
  //Disable pullup resistors
  digitalWrite(BUTTON_PIN, 0);

  //LED pin as output
  pinMode(LED_PIN, OUTPUT);
  //LED off
  digitalWrite(LED_PIN, HIGH);
  
  Serial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!Serial && millis() < 10000);

  Serial.println("Starting the push button switch example");

  //Attach an interrupt to the button pin - fire when button pressed down.
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressedISR, FALLING);
}

void loop() {
  //When the button is pressed, the pin is connected to ground, so we will read a 0.
  if(!digitalRead(BUTTON_PIN))
  {
    Serial.println("The push button is pressed.");
  }
  else
  {
    Serial.println("The push button is NOT pressed.");
  }
  delay(1000);
}

void buttonPressedISR()
{
  //Toggle LED when button is pressed
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

