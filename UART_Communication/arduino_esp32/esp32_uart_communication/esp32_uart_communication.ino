void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); //RX2 = GPIO16, TX2 = 17
}

void loop()
{ 
  Serial2.println("Hello!");
  delay(1000);
}
