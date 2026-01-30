#pragma once
#include <Arduino.h>
#include "driver/twai.h"

#define ID_HVAC 0x091
#define ID_WINDOW 0x0A1
#define ID_DOMELIGHT 0x0A2

// HVAC
#define HVAC_AC_ON 0x01
#define HVAC_AC_OFF 0x02
#define HVAC_TEMP_UP 0x03
#define HVAC_TEMP_DOWN 0x04
#define HVAC_SET_TEMP 0x05
#define HVAC_FAN_UP 0x06
#define HVAC_FAN_DOWN 0x07

// Window
#define WIN_OPEN 0x01
#define WIN_CLOSE 0x02
#define WIN_STOP 0x03
#define WIN_SET_POS 0x04

// Window Position IDs
#define W_ALL 0x00
#define W_FL 0x01
#define W_FR 0x02
#define W_BL 0x03
#define W_BR 0x04

// Domelight
#define DL_OPEN 0x01
#define DL_CLOSE 0x02

struct CarData
{
    // AC
    bool ac_status = false;
    uint8_t temp = 25;
    uint8_t fan_level = 1;

    // Window
    uint8_t win_fl = 0;
    uint8_t win_fr = 0;
    uint8_t win_bl = 0;
    uint8_t win_br = 0;

    // Domelight
    uint8_t dl_status = false;
};
extern volatile CarData currentCarState;
void Can_init();
void Rx_Can(void *pv);