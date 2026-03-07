#pragma once
#include <Arduino.h>
#include "driver/twai.h"

/* ================= CAN IDs ================= */
#define ID_HVAC 0x245
#define ID_WINDOW 0x325
#define ID_DOMELIGHT 0x420 // คงไว้ถ้ามีใช้

/* ================= HVAC ================= */
#define HVAC_CMD_SET 0x01

#define HVAC_MODE_AC_ON 0x01
#define HVAC_MODE_AUTO 0x02
#define HVAC_MODE_DEFROST 0x04

/* ================= WINDOW ================= */
#define WINDOW_FL 0x01
#define WINDOW_FR 0x02
#define WINDOW_RL 0x04
#define WINDOW_RR 0x08

#define WINDOW_MOVE 0x01

/* ================= DOMELIGHT ================= */
#define DL_OPEN 0x01
#define DL_CLOSE 0x00

/* ================= CAR STATE ================= */
struct CarData
{
    // HVAC
    bool ac_status = false;
    bool auto_mode = false;
    bool defrost = false;
    uint8_t temp = 25;     // °C
    uint8_t fan_level = 1; // 0–7

    // Window (0–100 %)
    uint8_t win_fl = 0;
    uint8_t win_fr = 0;
    uint8_t win_bl = 0;
    uint8_t win_br = 0;

    // Domelight
    bool dl_status = false;
};

extern volatile CarData currentCarState;

void Can_init();
void Rx_Can(void *pv);
