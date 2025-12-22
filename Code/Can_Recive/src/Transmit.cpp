#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

byte led[2];
bool ledState = false;
unsigned long lastTime = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Transmit");
  while (CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) != CAN_OK)
  {
    delay(100);
  }
  CAN.setMode(MCP_NORMAL);
}

void loop()
{
  if (millis() - lastTime >= 2000)
  {
    lastTime = millis();
    ledState = !ledState;

    led[0] = ledState ? 1 : 0;
    led[1] = ledState ? 255 : 0;

    CAN.sendMsgBuf(0x130, 0, 2, led);
  }
}
