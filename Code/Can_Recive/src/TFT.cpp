#include <TFT_eSPI.h>

TFT_eSPI tft;

void drawButton(int x, int y, int w, int h, int r,
                uint16_t btnColor, const char* label) {
  tft.fillRoundRect(x, y, w, h, r, btnColor);
  tft.setTextColor(TFT_BLACK, btnColor);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString(label, x + w / 2, y + h / 2);
}

void setup() {
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLUE);

  int btnW = 140;
  int btnH = 80;
  int radius = 18;

  int gapX = 40;
  int gapY = 30;

  int totalW = btnW * 2 + gapX;
  int totalH = btnH * 2 + gapY;

  int startX = (tft.width() - totalW) / 2;
  int startY = (tft.height() - totalH) / 2;

  int x1 = startX;
  int x2 = startX + btnW + gapX;
  int y1 = startY;
  int y2 = startY + btnH + gapY;

  drawButton(x1, y1, btnW, btnH, radius, TFT_RED, "ENG");
  drawButton(x2, y1, btnW, btnH, radius, TFT_RED, "CHINESE");
  drawButton(x1, y2, btnW, btnH, radius, TFT_RED, "THAI");
  drawButton(x2, y2, btnW, btnH, radius, TFT_RED, "KMITL");
}

void loop() {}
