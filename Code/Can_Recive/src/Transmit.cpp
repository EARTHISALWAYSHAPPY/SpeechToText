#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

byte led[2];

void setup()
{
  Serial.begin(9600);

  while (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
  {
    delay(100);
  }
  CAN.setMode(MCP_NORMAL);
}

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();
    int en = Serial.parseInt();
    int pwm = Serial.parseInt();

    if (cmd == 'L')
    {
      led[0] = constrain(en, 0, 1);
      led[1] = constrain(pwm, 0, 255);
      CAN.sendMsgBuf(0x130, 0, 2, led);
    }

    while (Serial.available())
      Serial.read();
  }
}
