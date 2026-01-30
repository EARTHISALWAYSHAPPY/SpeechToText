#include "TFT.h"
#include "../CAN/CAN.h"

TFT_eSPI tft;

struct Box
{
    int x, y, w, h;
};
Box winFL = {24, 98, 80, 18};
Box winBL = {24, 163, 80, 18};
Box winFR = {216, 98, 80, 18};
Box winBR = {216, 163, 80, 18};
Box air = {120, 85, 80, 21};
Box dome = {120, 130, 80, 21};

void drawBox(const Box &b, const String &txt, uint16_t col, uint16_t bg);
// void simulate();

void TFT_init()
{

    pinMode(OUTPUT, 5);
    digitalWrite(5, LOW);
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
    tft.pushImage(0, 0, FRAME_1_WIDTH, FRAME_1_HEIGHT, Frame_1);
    delay(3000);
    tft.pushImage(0, 0, FRAME_2_WIDTH, FRAME_2_HEIGHT, Frame_2);
}

void Display(void *pv)
{
    uint32_t lastMs = 0;
    for (;;)
    {
        if (millis() - lastMs >= 200)
        {
            lastMs = millis();

            int fl_pct = (currentCarState.win_fl * 100) / 15;
            int fr_pct = (currentCarState.win_fr * 100) / 15;
            int bl_pct = (currentCarState.win_bl * 100) / 15;
            int br_pct = (currentCarState.win_br * 100) / 15;

            drawBox(winFL, "FL:" + String(fl_pct) + "%", TFT_GREEN, TFT_BLACK);
            drawBox(winFR, "FR:" + String(fr_pct) + "%", TFT_GREEN, TFT_BLACK);
            drawBox(winBL, "BL:" + String(bl_pct) + "%", TFT_GREEN, TFT_BLACK);
            drawBox(winBR, "BR:" + String(br_pct) + "%", TFT_GREEN, TFT_BLACK);

            String ac_txt = currentCarState.ac_status ? "AC:ON" : "AC:OFF";
            uint16_t ac_col = currentCarState.ac_status ? TFT_CYAN : TFT_DARKGREY;

            drawBox(air, ac_txt + " " + String(currentCarState.temp) + "C", ac_col, TFT_BLACK);

            drawBox(dome, "LIGHT:" + String(currentCarState.dl_status), TFT_ORANGE, TFT_BLACK);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
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
