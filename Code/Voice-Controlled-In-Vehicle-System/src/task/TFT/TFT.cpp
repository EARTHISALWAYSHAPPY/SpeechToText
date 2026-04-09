#include "TFT.h"
#include "../CAN/CAN.h"

#define LED_MASK ((1 << 12) | (1 << 14))
#define LED1_MASK ((1 << (32 - 32)) | (1 << (33 - 32)))

TFT_eSPI tft;

Box winFL_bar = {99, 50, 14, 50};
Box winBL_bar = {99, 107, 14, 50};
Box winFR_bar = {207, 50, 14, 50};
Box winBR_bar = {207, 107, 14, 50};
Box air = {70, 201, 81, 26};
Box dome = {120, 130, 80, 21};

// // test
Box Box_Number_AC = {70, 201, 81, 26};

void drawBox(const Box &b, const String &txt, uint16_t col, uint16_t bg);
void drawWindowBar(const Box &b, int percent);
// void simulate();

void TFT_init()
{
    PIN_FUNC_SELECT(IO_MUX_GPIO12_REG, PIN_FUNC_GPIO);
    PIN_FUNC_SELECT(IO_MUX_GPIO14_REG, PIN_FUNC_GPIO);
    PIN_FUNC_SELECT(IO_MUX_GPIO32_REG, PIN_FUNC_GPIO);
    PIN_FUNC_SELECT(IO_MUX_GPIO33_REG, PIN_FUNC_GPIO);
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
    int last_temp = -1;
    int last_light = -1;

    static int disp_fl = 0;
    static int disp_fr = 0;
    static int disp_bl = 0;
    static int disp_br = 0;
    static bool first_draw_window = true;

    static int disp_temp = 25;
    static bool first_draw_ac = true;

    for (;;)
    {
        // ---------- Window ----------
        if (currentCarState.win_fl != last_fl || currentCarState.win_fr != last_fr || currentCarState.win_bl != last_bl || currentCarState.win_br != last_br ||
            disp_fl != last_fl || disp_fr != last_fr || disp_bl != last_bl || disp_br != last_br)
        {
            uint8_t step = 5;
            last_fl = currentCarState.win_fl;
            last_fr = currentCarState.win_fr;
            last_bl = currentCarState.win_bl;
            last_br = currentCarState.win_br;
            bool redraw_fl = false;
            bool redraw_fr = false;
            bool redraw_bl = false;
            bool redraw_br = false;
            if (first_draw_window)
            {
                disp_fl = last_fl;
                disp_fr = last_fr;
                disp_bl = last_bl;
                disp_br = last_br;
                first_draw_window = false;
            }
            else
            {
                // FL
                if (disp_fl != last_fl)
                {
                    disp_fl += (disp_fl < last_fl) ? step : -step;
                }

                // FR
                if (disp_fr != last_fr)
                {
                    disp_fr += (disp_fr < last_fr) ? step : -step;
                }

                // BL
                if (disp_bl != last_bl)
                {
                    disp_bl += (disp_bl < last_bl) ? step : -step;
                }

                // BR
                if (disp_br != last_br)
                {
                    disp_br += (disp_br < last_br) ? step : -step;
                }
            }
            drawWindowBar(winFL_bar, disp_fl);
            drawWindowBar(winFR_bar, disp_fr);
            drawWindowBar(winBL_bar, disp_bl);
            drawWindowBar(winBR_bar, disp_br);
        }

        // ---------- AC ----------
        if (currentCarState.ac_status != last_ac ||
            currentCarState.temp != last_temp ||
            disp_temp != currentCarState.temp)
        {

            uint16_t ac_col = currentCarState.ac_status ? TFT_GREEN : TFT_RED;

            switch (currentCarState.ac_status)
            {
            case 1:
                tft.pushImage(28, 201, FAN_ON_WIDTH, FAN_ON_HEIGHT, Fan_on);
                break;
            default:
                tft.pushImage(28, 201, FAN_OFF_WIDTH, FAN_OFF_HEIGHT, Fan_off);
                break;
            }

            if (first_draw_ac)
            {
                disp_temp = currentCarState.temp;
                first_draw_ac = false;
            }
            else
            {
                disp_temp = (disp_temp < currentCarState.temp) ? disp_temp + 1 : (disp_temp > currentCarState.temp) ? disp_temp - 1
                                                                                                                    : disp_temp;
            }

            drawBox(Box_Number_AC, String(disp_temp) + " C", ac_col, TFT_BLACK);

            last_ac = currentCarState.ac_status;
            last_temp = currentCarState.temp;
        }

        // ---------- LIGHT ----------
        if (currentCarState.dl_status != last_light)
        {
            switch (currentCarState.dl_status)
            {
            case true:

                tft.pushImage(288, 18, LIGHT_ON_WIDTH, LIGHT_ON_HEIGHT, Light_on);
                REG_WRITE(GPIO_OUT_W1TS_REG, LED_MASK);
                REG_WRITE(GPIO_OUT1_W1TS_REG, LED1_MASK);
                break;

            default:

                tft.pushImage(288, 18, LIGHT_OFF_WIDTH, LIGHT_OFF_HEIGHT, Light_off);
                REG_WRITE(GPIO_OUT_W1TC_REG, LED_MASK);
                REG_WRITE(GPIO_OUT1_W1TC_REG, LED1_MASK);
                break;
            }

            last_light = currentCarState.dl_status;
        }

        vTaskDelay(pdMS_TO_TICKS(150));
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

void drawWindowBar(const Box &b, int percent)
{
    int bar = percent * b.h / 100;

    uint16_t border = (percent == 0) ? TFT_RED : TFT_GREEN;

    // clear
    tft.fillRect(b.x, b.y, b.w, b.h, TFT_BLACK);

    // bar
    if (bar > 0)
        tft.fillRect(b.x, b.y + (b.h - bar), b.w, bar, TFT_GREEN);

    // border
    tft.drawRoundRect(b.x, b.y, b.w, b.h, 3, border);
}