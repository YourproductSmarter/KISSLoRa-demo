#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// The KISS device should only be used in Europe
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  // Set callback for incoming messages
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  delay(30000); //remove this after you registered your device

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  // Seed the random function
  seedRandomFunction();
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Send single byte to poll for incoming messages, using port 1
  ttn.poll(1, false);

  delay(10000);

  // Prepare payload of 1 byte to indicate rotary switch position
  byte payload[1];
  payload[0] = getRotaryPosition();

  // Send the payload, on port 2, not asking for an ack, using the random SF
  ttn.sendBytes(payload, sizeof(payload), 2, false, getSF());

  delay(10000);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < size; i++)
  {
    debugSerial.print(" " + String(payload[i]));
  }

  debugSerial.println();

  //Toggle red LED when a message is received
  #define LED_PIN 12
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

//Poll the rotary switch
int8_t getRotaryPosition()
{
  #define ROTARY_PIN_0 5
  #define ROTARY_PIN_1 13
  #define ROTARY_PIN_2 9
  #define ROTARY_PIN_3 30

  //Set pins as inputs
  pinMode(ROTARY_PIN_0, INPUT);
  pinMode(ROTARY_PIN_1, INPUT);
  pinMode(ROTARY_PIN_2, INPUT);
  pinMode(ROTARY_PIN_3, INPUT);
  //Disable pullup resistors
  digitalWrite(ROTARY_PIN_0, 0);
  digitalWrite(ROTARY_PIN_1, 0);
  digitalWrite(ROTARY_PIN_2, 0);
  digitalWrite(ROTARY_PIN_3, 0);

  int8_t value = 0;
  if (digitalRead(ROTARY_PIN_0)) {value |= 1 << 0;}
  if (digitalRead(ROTARY_PIN_1)) {value |= 1 << 1;}
  if (digitalRead(ROTARY_PIN_2)) {value |= 1 << 2;}
  if (digitalRead(ROTARY_PIN_3)) {value |= 1 << 3;}
  
  //Set value to -1 if it's higher than 9 - this should never happen but just in case
  if (value > 9)
  {
    value = -1;
  }

  return value;
}

void seedRandomFunction()
{
  char hwEui_char_array[16+1];//16 chars + \0
  uint16_t hwEui_16_bits = 0;
  
  ttn.getHardwareEui(hwEui_char_array, 17); //read HWEUI from module
  hwEui_16_bits = ascii_hex_to_nibble(hwEui_char_array[12]);
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[13]) << 4;
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[14]) << 8;
  hwEui_16_bits |= ascii_hex_to_nibble(hwEui_char_array[15]) << 12;
  uint16_t seed = hwEui_16_bits;
  randomSeed(seed);
}

uint8_t getSF()
{
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
}

//This function is used to convert ascii-hex string to integer
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
