#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 11

MCP_CAN CAN(CAN_CS);

long unsigned int canId;
unsigned char canLen;
unsigned char canBuf[8];

void servo_init()
{
    DDRB |= (1 << DDB2);

    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1A |= (1 << COM1B1);
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    TCCR1B |= (1 << CS11);

    ICR1 = 40000;

    OCR1B = 3000;
}

void servo_write_angle(unsigned char angle)
{
    if (angle > 180)
        angle = 180;

    unsigned int us = 1000 + ((unsigned long)angle * 1000UL) / 180UL;
    unsigned int ticks = us * 2;

    OCR1B = ticks;
}

void setup()
{
    Serial.begin(9600);

    servo_init();

    while (CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) != CAN_OK)
    {
        delay(100);
    }
    CAN.setMode(MCP_NORMAL);

    CAN.init_Mask(0, 0, 0x7FF);
    CAN.init_Filt(0, 0, 0x0A1);
}

void loop()
{
    if (CAN.checkReceive() == CAN_MSGAVAIL)
    {
        CAN.readMsgBuf(&canId, &canLen, canBuf);

        if (canId == 0x0A1 && canLen >= 2)
        {
            unsigned char enable = canBuf[0];
            unsigned char angle = canBuf[1];

            if (enable)
            {
                servo_write_angle(angle);
            }
        }
    }
}