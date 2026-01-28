#include <mcp_can.h>
#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

void Can_init()
{
    CAN.setMode(MCP_NORMAL);
    CAN.init_Mask(0, 0, 0x7F0);
    CAN.init_Filt(0, 0, 0x130);
}
void Rx_Can(void *pv)
{

    long unsigned int canId;
    unsigned char canLen;
    unsigned char canBuf[8];
    for (;;)
    {
        if (CAN.checkReceive() == CAN_MSGAVAIL)
        {
            CAN.readMsgBuf(&canId, &canLen, canBuf);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}