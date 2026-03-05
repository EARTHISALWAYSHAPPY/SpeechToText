#include "test_logic.h"

static int displayed_temp = 25; 
static int target_temp    = 25;
static bool first_draw    = true;

/// animation ac ///  
void TestLogic_Init_ac(void)
{
    Serial.begin(115200);
    Serial.println("=== TEST LOGIC MODE ===");
    Serial.println("Type temperature (16-40) and press Enter");
}

static void readTempFromSerial()
{
    if (!Serial.available()) return;

    int val = Serial.parseInt();

    if (val >= 16 && val <= 40)
    {
        target_temp = val;
    }

    while (Serial.available()) Serial.read();
}

void TestLogic_Update(const Box &box, bool ac_status)
{
    readTempFromSerial();

    if (first_draw)
    {
        first_draw = false;
    }
    else
    {
        if (displayed_temp < target_temp)
            displayed_temp++;
        else if (displayed_temp > target_temp)
            displayed_temp--;
        else
            return; 
    }

    uint16_t col = ac_status ? TFT_GREEN : TFT_RED;
    drawBox(box, String(displayed_temp) + " C", col, TFT_BLACK);
}
//////////////////

/// animation window /// 
static int fl = 0;
static int fr = 0;
static int bl = 0;
static int br = 0;

static int disp_fl = 0;
static int disp_fr = 0;
static int disp_bl = 0;
static int disp_br = 0;


static void drawWindowBar(TFT_eSPI &tft, int x, int y, int w, int h, int percent)
{
    int bar = map(percent, 0, 100, 0, h);

    // clear
    tft.fillRect(x, y, w, h, TFT_BLACK);

    // border
    tft.drawRect(x, y, w, h, TFT_WHITE);

    // fill
    tft.fillRect(x, y + (h - bar), w, bar, TFT_CYAN);
}


void TestLogic_Init_window()
{
    Serial.begin(115200);

    Serial.println("=== WINDOW TEST MODE ===");
    Serial.println("Example:");
    Serial.println("fl 80");
    Serial.println("fr 20");
    Serial.println("bl 100");
    Serial.println("br 50");
}


void TestLogic_ReadSerial()
{
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    int val;

    if (cmd.startsWith("fl"))
    {
        val = cmd.substring(2).toInt();
        fl = constrain(val, 0, 100);
    }
    else if (cmd.startsWith("fr"))
    {
        val = cmd.substring(2).toInt();
        fr = constrain(val, 0, 100);
    }
    else if (cmd.startsWith("bl"))
    {
        val = cmd.substring(2).toInt();
        bl = constrain(val, 0, 100);
    }
    else if (cmd.startsWith("br"))
    {
        val = cmd.substring(2).toInt();
        br = constrain(val, 0, 100);
    }
}


void TestLogic_UpdateWindow(
    TFT_eSPI &tft,
    const Box &fl_box,
    const Box &fr_box,
    const Box &bl_box,
    const Box &br_box)
{
    // animation FL
    if (disp_fl < fl) disp_fl++;
    else if (disp_fl > fl) disp_fl--;

    drawWindowBar(tft, fl_box.x, fl_box.y, fl_box.w, fl_box.h, disp_fl);


    // animation FR
    if (disp_fr < fr) disp_fr++;
    else if (disp_fr > fr) disp_fr--;

    drawWindowBar(tft, fr_box.x, fr_box.y, fr_box.w, fr_box.h, disp_fr);


    // animation BL
    if (disp_bl < bl) disp_bl++;
    else if (disp_bl > bl) disp_bl--;

    drawWindowBar(tft, bl_box.x, bl_box.y, bl_box.w, bl_box.h, disp_bl);


    // animation BR
    if (disp_br < br) disp_br++;
    else if (disp_br > br) disp_br--;

    drawWindowBar(tft, br_box.x, br_box.y, br_box.w, br_box.h, disp_br);
}
//////////////////