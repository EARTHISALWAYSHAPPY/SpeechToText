#include "CAN.h"

#define TX_PIN 25
#define RX_PIN 26

void Can_init()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
    {
        twai_start();
    }
    else
    {
        Serial.println("CAN Init Failed!");
    }
}

void Rx_Can(void *pv)
{
    twai_message_t message;

    for (;;)
    {
        if (twai_receive(&message, portMAX_DELAY) != ESP_OK)
            continue;

        // Debug: ID 0x64 (Hex) คือ 100 (Dec)
        Serial.printf("ID: 0x%03X | DLC: %d | Data: ", message.identifier, message.data_length_code);
        for (int i = 0; i < message.data_length_code; i++)
        {
            Serial.printf("%02X ", message.data[i]);
        }
        Serial.println();

        if (message.identifier == ID_HVAC && message.data_length_code == 8)
        {
            uint8_t cmd = message.data[0];
            if (cmd == HVAC_CMD_SET)
            {
                currentCarState.temp = message.data[1] - 40;
                currentCarState.fan_level = message.data[2] & 0x0F;
                currentCarState.ac_status = message.data[3] & HVAC_MODE_AC_ON;
                currentCarState.auto_mode = message.data[3] & HVAC_MODE_AUTO;
                currentCarState.defrost = message.data[3] & HVAC_MODE_DEFROST;
            }
        }
        else if (message.identifier == ID_WINDOW && message.data_length_code == 8)
        {
            uint8_t mask = message.data[0];
            uint8_t pos = message.data[1]; // รับค่า 0x64 (100) มาตรงๆ
            uint8_t cmd = message.data[3];

            if (cmd == WINDOW_MOVE)
            {
                if (mask & WINDOW_FL)
                    currentCarState.win_fl = pos;
                if (mask & WINDOW_FR)
                    currentCarState.win_fr = pos;
                if (mask & WINDOW_RL)
                    currentCarState.win_bl = pos;
                if (mask & WINDOW_RR)
                    currentCarState.win_br = pos;
            }
        }
        else if (message.identifier == ID_DOMELIGHT)
        {
            currentCarState.dl_status = (message.data[0] == DL_OPEN);
        }
    }
}