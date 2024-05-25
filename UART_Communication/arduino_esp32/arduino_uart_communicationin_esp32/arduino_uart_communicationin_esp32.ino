#include<SoftwareSerial.h>
SoftwareSerial SUART(10, 11); //SRX = DPin-10, STX = 17

void setup()
{
  Serial.begin(9600);
  SUART.begin(9600);
}

void loop()
{
  while (SUART.available() != 0)
  {
    char ch = SUART.read();
    Serial.print(ch);
  }
}
