#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

long unsigned int canId;
unsigned char canLen;
unsigned char canBuf[8];

void receiveCAN();
void printFrame();
void handleFrame();

void setup()
{
    Serial.begin(9600);

    // Init CAN
    // while (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
    // {
    //     delay(100);
    // }
    CAN.setMode(MCP_NORMAL);

    CAN.init_Mask(0, 0, 0x7F0);
    CAN.init_Filt(0, 0, 0x110);
    CAN.init_Filt(1, 0, 0x120);

    Serial.println("Setting done");
}
void loop()
{
    receiveCAN();
}

void receiveCAN()
{
    if (CAN.checkReceive() == CAN_MSGAVAIL)
    {
        CAN.readMsgBuf(&canId, &canLen, canBuf);

        printFrame();
        handleFrame();
    }
}

void printFrame()
{
    Serial.print("ID=0x");
    Serial.print(canId, HEX);
    Serial.print(" DLC=");
    Serial.print(canLen);
    Serial.print(" DATA=");

    for (int i = 0; i < canLen; i++)
    {
        if (canBuf[i] < 0x10)
            Serial.print("0");
        Serial.print(canBuf[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void handleFrame()
{
    if (canId == 0x110 && canLen >= 2)
    {
        // Fan command
    }

    if (canId == 0x120 && canLen >= 2)
    {
        // Servo command
    }
}
