#pragma once
#include <Arduino.h>
#include "task/TFT/TFT.h"

// init test logic (Serial)
void TestLogic_Init(void);
// update animation
void TestLogic_Update(const Box &box, bool ac_status);