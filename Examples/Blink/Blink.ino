/*
  Blink.ino - consecutively blink all LEDs on the KISS LoRa board
  
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

#define LED_RED   12
#define LED_GREEN 6
#define LED_BLUE  11
#define LED_LORA  8

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_LORA, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_RED, LOW);   //switch red LED on
  delay(500);                   //delay 500 milliseconds
  digitalWrite(LED_RED, HIGH);  //switch red LED off
  digitalWrite(LED_GREEN, LOW);
  delay(500);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
  delay(500);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_LORA, LOW);
  delay(500);
  digitalWrite(LED_LORA, HIGH);
}
