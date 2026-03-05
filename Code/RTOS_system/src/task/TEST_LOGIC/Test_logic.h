#pragma once
#include <Arduino.h>
#include "task/TFT/TFT.h"

// init test logic (Serial)
void TestLogic_Init_ac(void);
// update animation AC
void TestLogic_Update(const Box &box, bool ac_status);

void TestLogic_Init_window(void);
void TestLogic_ReadSerial(void);
// update animation window
void TestLogic_UpdateWindow(TFT_eSPI &tft,const Box &fl,const Box &fr,const Box &bl,const Box &br);