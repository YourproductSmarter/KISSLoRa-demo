#include "IntemoPM.h"

#define joseneAddress 0x4E

JosenePM JosenePM;

void setup()
{
  Serial.begin(9600);

  JosenePM.begin(joseneAddress, 1);
  JosenePM.powerOn();
}
void loop()
{
  uint16_t *data = JosenePM.getData(false);
  Serial.print("Josene PM Data:\nPM10: ");
  Serial.println(data[0]);
  Serial.print("PM2.5: ");
  Serial.println(data[1]);
  delay(2000);
}
