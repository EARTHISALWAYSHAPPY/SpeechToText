#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10
#define LED_PIN 5

MCP_CAN CAN(CAN_CS);

long unsigned int canId;
unsigned char canLen;
unsigned char canBuf[8];

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Recive");
  while (CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) != CAN_OK)
  {
    delay(100);
  }
  CAN.setMode(MCP_NORMAL);

  CAN.init_Mask(0, 0, 0x7F0);
  CAN.init_Filt(0, 0, 0x130);
}

void loop()
{
  if (CAN.checkReceive() == CAN_MSGAVAIL)
  {
    CAN.readMsgBuf(&canId, &canLen, canBuf);

    if (canId == 0x130 && canLen >= 2)
    {
      byte enable = canBuf[0];
      byte pwm = canBuf[1];

      if (enable)
        analogWrite(LED_PIN, pwm);
      else
        analogWrite(LED_PIN, 0);
    }
  }
}

//HMI :  
//    Window Status : 0x0A2
//    Air Status : 0x092
//Window : 0x0A1
//Air : 0x091