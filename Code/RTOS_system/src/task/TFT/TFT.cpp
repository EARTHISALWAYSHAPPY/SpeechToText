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
Box air = {70, 201, 81, 26}; // fix
Box dome = {120, 130, 80, 21};

// test
Box customBox = {70, 201, 81, 26};

void drawBox(const Box &b, const String &txt, uint16_t col, uint16_t bg);
// void simulate();

void TFT_init()
{
    REG_WRITE(GPIO_ENABLE_REG, REG_READ(GPIO_ENABLE_REG) | (1 << 12) | (1 << 14));
    REG_WRITE(GPIO_ENABLE1_REG, REG_READ(GPIO_ENABLE1_REG) | (1 << (32 - 32)) | (1 << (33 - 32)));
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);

    tft.pushImage(0, 0, FRAME_1_WIDTH, FRAME_1_HEIGHT, Frame_1);
    delay(3000);

    tft.pushImage(0, 0, FRAME_2_WIDTH, FRAME_2_HEIGHT, Frame_2);

    // tft.pushImage(29, 200, FAN_OFF_WIDTH, FAN_OFF_HEIGHT, Fan_off);
    //  drawBox(customBox, "TEST", TFT_WHITE, TFT_BLACK);
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
        // int fl_pct = (int)currentCarState.win_fl;
        // int fr_pct = (int)currentCarState.win_fr;
        // int bl_pct = (int)currentCarState.win_bl;
        // int br_pct = (int)currentCarState.win_br;

        // // ---------- WINDOW ----------
        // if (fl_pct != last_fl)
        // {
        //     drawBox(winFL, "FL:" + String(fl_pct) + "%", TFT_GREEN, TFT_BLACK);
        //     last_fl = fl_pct;
        // }

        // if (fr_pct != last_fr)
        // {
        //     drawBox(winFR, "FR:" + String(fr_pct) + "%", TFT_GREEN, TFT_BLACK);
        //     last_fr = fr_pct;
        // }

        // if (bl_pct != last_bl)
        // {
        //     drawBox(winBL, "BL:" + String(bl_pct) + "%", TFT_GREEN, TFT_BLACK);
        //     last_bl = bl_pct;
        // }

        // if (br_pct != last_br)
        // {
        //     drawBox(winBR, "BR:" + String(br_pct) + "%", TFT_GREEN, TFT_BLACK);
        //     last_br = br_pct;
        // }

        // ---------- AC ----------
        if (currentCarState.ac_status != last_ac ||
            currentCarState.temp != last_temp)
        {
            // String ac_txt = currentCarState.ac_status ? "AC:ON" : "AC:OFF";
            uint16_t ac_col = currentCarState.ac_status ? TFT_GREEN : TFT_RED;
            switch (currentCarState.ac_status)
            {
            case 1:
                tft.pushImage(29, 200, FAN_ON_WIDTH, FAN_ON_HEIGHT, Fan_on);
                break;
            default:
                tft.pushImage(29, 200, FAN_OFF_WIDTH, FAN_OFF_HEIGHT, Fan_off);
                break;
            }

            drawBox(customBox, String(currentCarState.temp) + "C", ac_col, TFT_BLACK);

            last_ac = currentCarState.ac_status;
            last_temp = currentCarState.temp;
        }

        // ---------- LIGHT ----------
        if (currentCarState.dl_status != last_light)
        {
            switch (currentCarState.dl_status)
            {
            case true:
                tft.pushImage(155, 97, LIGHT_ON_WIDTH, LIGHT_ON_HEIGHT, Light_on);
                REG_WRITE(GPIO_OUT_W1TS_REG, (1 << 12) | (1 << 14));
                REG_WRITE(GPIO_OUT1_W1TS_REG, (1 << (32 - 32)) | (1 << (33 - 32)));
                break;
            default:
                tft.pushImage(155, 97, LIGHT_OFF_WIDTH, LIGHT_OFF_HEIGHT, Light_off);
                REG_WRITE(GPIO_OUT_W1TC_REG, (1 << 12) | (1 << 14));
                REG_WRITE(GPIO_OUT1_W1TC_REG, (1 << (32 - 32)) | (1 << (33 - 32)));
                break;
            }

            last_light = currentCarState.dl_status;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void drawBox(const Box &b, const String &txt, uint16_t col, uint16_t bg)
{
    tft.fillRect(b.x, b.y, b.w, b.h, bg);
    tft.drawRoundRect(b.x, b.y, b.w, b.h, 3, col);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(col, bg);
    tft.setTextSize(2);
    tft.drawString(txt, b.x + b.w / 2, b.y + b.h / 2);
}
