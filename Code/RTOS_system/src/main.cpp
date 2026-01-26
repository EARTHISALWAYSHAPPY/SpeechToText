#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>
#include <TFT_eSPI.h>
#include "Frame_1.h"
#include "Frame_2.h"

#define CAN_CS 10

// MCP_CAN CAN(CAN_CS);

TFT_eSPI tft;

struct Box
{
  int x, y, w, h;
};

Box winFL = {24, 98, 80, 18};
Box winRL = {24, 163, 80, 18};
Box winFR = {216, 98, 80, 18};
Box winRR = {216, 163, 80, 18};
Box air = {120, 85, 80, 21};
Box dome = {120, 130, 80, 21};
uint8_t fl = 75, rl = 50, rr = 100;
bool fr_on = false, dome_on = true;
uint8_t temp = 25;
int dirFL = 1, dirRL = 1, dirRR = -1;

// void Rx_Can(void *pvParameters);
void Display(void *pvParameters);
void drawBox(const Box &b, const String &txt, uint16_t col, uint16_t bg);
void simulate();

void setup()

{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, FRAME_1_WIDTH, FRAME_1_HEIGHT, Frame_1);
  delay(3000);
  tft.pushImage(0, 0, FRAME_2_WIDTH, FRAME_2_HEIGHT, Frame_2);
  // CAN.setMode(MCP_NORMAL);
  // CAN.init_Mask(0, 0, 0x7F0);
  // CAN.init_Filt(0, 0, 0x130);
  // xTaskCreate(Rx_Can, "Recive_Can", 4096, NULL, 1, NULL);
  xTaskCreate(Display, "Display_TFT", 4096, NULL, 1, NULL);
}

void loop()
{
  vTaskDelete(NULL);
}

// void Rx_Can(void *pvParameters)
// {

//   long unsigned int canId;
//   unsigned char canLen;
//   unsigned char canBuf[8];
//   for (;;)
//   {
//     if (CAN.checkReceive() == CAN_MSGAVAIL)
//     {
//       CAN.readMsgBuf(&canId, &canLen, canBuf);
//     }
//     //vTaskDelay(pdMS_TO_TICKS(10));
//   }
// }

void Display(void *pv)
{

  uint32_t lastMs = 0;
  for (;;)
  {
    if (millis() - lastMs >= 500)
    {
      lastMs += 500;
      simulate();
      drawBox(winFL, "ON:" + String(fl) + "%", TFT_GREEN, TFT_BLACK);
      drawBox(winRL, "ON:" + String(rl) + "%", TFT_GREEN, TFT_BLACK);
      drawBox(winRR, "ON:" + String(rr) + "%", TFT_GREEN, TFT_BLACK);
      drawBox(winFR, fr_on ? "ON:40%" : "OFF",
              fr_on ? TFT_GREEN : TFT_RED, TFT_BLACK);
      drawBox(air, "ON:" + String(temp) + "C", TFT_GREEN, TFT_WHITE);
      drawBox(dome, dome_on ? "ON" : "OFF",
              dome_on ? TFT_GREEN : TFT_RED, TFT_WHITE);
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void drawBox(const Box &b, const String &txt, uint16_t col, uint16_t bg)
{
  tft.fillRect(b.x, b.y, b.w, b.h, bg);
  tft.drawRoundRect(b.x, b.y, b.w, b.h, 9, col);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(col, bg);
  tft.setTextSize(1);
  tft.drawString(txt, b.x + b.w / 2, b.y + b.h / 2);
}

void simulate()

{
  fl = (uint8_t)constrain((int)fl + dirFL * 5, 0, 100);
  if (fl == 100 || fl == 0)
    dirFL *= -1;
  rl = (uint8_t)constrain((int)rl + dirRL * 4, 0, 100);
  if (rl == 100 || rl == 0)
    dirRL *= -1;
  rr = (uint8_t)constrain((int)rr + dirRR * 6, 0, 100);
  if (rr == 100 || rr == 0)
    dirRR *= -1;
  static int t = 0;
  t++;
  if (t % 4 == 0)
    fr_on = !fr_on;
  if (t % 3 == 0)
    dome_on = !dome_on;
  temp++;
  if (temp > 28)
    temp = 22;
}