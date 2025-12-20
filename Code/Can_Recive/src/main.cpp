#include <SPI.h>
#include <mcp_can.h>

MCP_CAN CAN_Action(10);  

void setup() {
  Serial.begin(9600);

  // while (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
  //   delay(100);
  // }

  CAN_Action.setMode(MCP_NORMAL);
}

void loop() {
  long unsigned int id;
  unsigned char len;
  unsigned char buf[8];

  if (CAN_Action.checkReceive() == CAN_MSGAVAIL) {
    // CAN_Action.readMsgBuf(&id, &len, buf);

    // Serial.print("ID=");
    // Serial.print(id, HEX);
    // Serial.print(" Data=");

    // for (int i = 0; i < len; i++) {
    //   Serial.print(buf[i], HEX);
    //   Serial.print(" ");
    // }
    // Serial.println();
  }
}
