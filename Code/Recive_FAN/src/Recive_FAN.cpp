#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 11

MCP_CAN CAN(CAN_CS);

long unsigned int canId;
unsigned char canLen;
unsigned char canBuf[8];

static inline void fan_pwm_init()
{
  DDRD |= (1 << DDD5);
  TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0B1);
  TCCR0B = (1 << CS01) | (1 << CS00);
  OCR0B = 0;
}

static inline void fan_pwm_set(unsigned char duty)
{
  OCR0B = duty;
}

static inline void fan_pwm_off()
{
  OCR0B = 0;
}

void setup()
{
  Serial.begin(9600);

  while (CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) != CAN_OK)
  {
    delay(100);
  }
  CAN.setMode(MCP_NORMAL);

  CAN.init_Mask(0, 0, 0x7FF);
  CAN.init_Filt(0, 0, 0x091);

  fan_pwm_init();
  fan_pwm_off();
}

void loop()
{
  if (CAN.checkReceive() == CAN_MSGAVAIL)
  {
    CAN.readMsgBuf(&canId, &canLen, canBuf);

    if (canId == 0x091 && canLen >= 2)
    {
      unsigned char enable = canBuf[0];
      unsigned char pwm = canBuf[1];

      if (enable)
        fan_pwm_set(pwm);
      else
        fan_pwm_off();
    }
  }
}