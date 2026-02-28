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
    tft.pushImage(29,200, FAN_OFF_WIDTH, FAN_OFF_HEIGHT, fan_on);
}

void Display(void *pv)
{
    int last_fl = -1;
    int last_fr = -1;
    int last_bl = -1;
    int last_br = -1;
    bool last_ac = false;
    int last_temp = -1000;
    int last_light = -1;

    for (;;)
    {
        int fl_pct = (int)currentCarState.win_fl;
        int fr_pct = (int)currentCarState.win_fr;
        int bl_pct = (int)currentCarState.win_bl;
        int br_pct = (int)currentCarState.win_br;

        // ---------- WINDOW ----------
        if (fl_pct != last_fl)
        {
            drawBox(winFL, "FL:" + String(fl_pct) + "%", TFT_GREEN, TFT_BLACK);
            last_fl = fl_pct;
        }

        if (fr_pct != last_fr)
        {
            drawBox(winFR, "FR:" + String(fr_pct) + "%", TFT_GREEN, TFT_BLACK);
            last_fr = fr_pct;
        }

        if (bl_pct != last_bl)
        {
            drawBox(winBL, "BL:" + String(bl_pct) + "%", TFT_GREEN, TFT_BLACK);
            last_bl = bl_pct;
        }

        if (br_pct != last_br)
        {
            drawBox(winBR, "BR:" + String(br_pct) + "%", TFT_GREEN, TFT_BLACK);
            last_br = br_pct;
        }

        // ---------- AC ----------
        if (currentCarState.ac_status != last_ac ||
            currentCarState.temp != last_temp)
        {
            // String ac_txt = currentCarState.ac_status ? "AC:ON" : "AC:OFF";
            if (currentCarState.ac_status == 1)
            {
                tft.pushImage(29, 200, FAN_ON_WIDTH, FAN_ON_HEIGHT, fan_on);
            }
            else
            {
                tft.pushImage(29,200, FAN_OFF_WIDTH, FAN_OFF_HEIGHT, fan_off);
            }

            last_ac = currentCarState.ac_status;
            last_temp = currentCarState.temp;
        }

        // ---------- LIGHT ----------
        if (currentCarState.dl_status != last_light)
        {
            drawBox(dome, "LIGHT:" + String(currentCarState.dl_status),
                    TFT_ORANGE, TFT_BLACK);

            last_light = currentCarState.dl_status;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
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
