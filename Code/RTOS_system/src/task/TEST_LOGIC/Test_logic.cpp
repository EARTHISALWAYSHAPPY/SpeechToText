#include "test_logic.h"

static int displayed_temp = 25; 
static int target_temp    = 25;
static bool first_draw    = true;

/// animation ac ///  
void TestLogic_Init(void)
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