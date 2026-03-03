#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "soc/gpio_reg.h"
#include "soc/io_mux_reg.h"
#include "gui/Frame_1.h"
#include "gui/Frame_2.h"
#include "gui/fan_on.h"
#include "gui/fan_off.h"
#include "gui/Light_off.h"
#include "gui/Light_on.h"

void TFT_init();
void Display(void *pv);