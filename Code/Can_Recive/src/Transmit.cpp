#include <SPI.h>
#include <mcp_can.h>

MCP_CAN CAN(10);

byte fan[2] = {1, 0};
byte servo[2] = {1, 0};

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
    int value = Serial.parseInt();

    if (cmd == 'F')
    {
      value = constrain(value, 0, 255);
      fan[1] = value;
      CAN.sendMsgBuf(0x110, 0, 2, fan);
    }

    if (cmd == 'S')
    {
      value = constrain(value, 0, 180);
      servo[1] = value;
      CAN.sendMsgBuf(0x120, 0, 2, servo);
    }

    while (Serial.available())
      Serial.read();
  }
}
