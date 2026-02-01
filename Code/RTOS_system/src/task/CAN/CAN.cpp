#include "CAN.h"

#define TX_PIN 5
#define RX_PIN 4

void Can_init()
{
    // Config
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
    {
        twai_start();
        Serial.println("CAN Started");
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
        if (twai_receive(&message, portMAX_DELAY) == ESP_OK)
        {
            // CASE 1: AC ID 0x091
            if (message.identifier == ID_HVAC)
            {
                uint8_t cmd = message.data[0];
                uint8_t val = message.data[1];

                switch (cmd)
                {
                case HVAC_AC_ON:
                    currentCarState.ac_status = true;
                    break;
                case HVAC_AC_OFF:
                    currentCarState.ac_status = false;
                    break;
                case HVAC_SET_TEMP:
                    currentCarState.temp = val;
                    break;

                case HVAC_TEMP_UP:
                    if (currentCarState.temp < 30)
                        currentCarState.temp += val;
                    break;
                case HVAC_TEMP_DOWN:
                    if (currentCarState.temp > 16)
                        currentCarState.temp -= val;
                    break;

                case HVAC_FAN_UP:
                    if (currentCarState.fan_level < 7)
                        currentCarState.fan_level++;
                    break;
                case HVAC_FAN_DOWN:
                    if (currentCarState.fan_level > 0)
                        currentCarState.fan_level--;
                    break;
                }
            }
            // CASE 2: WINDOW ID 0x0A1
            else if (message.identifier == ID_WINDOW)
            {
                uint8_t cmd = message.data[0];
                uint8_t raw_byte1 = message.data[1];

                uint8_t win_id = (raw_byte1 >> 4) & 0x0F; // 4 bit frist = ID
                uint8_t pos_val = raw_byte1 & 0x0F;       // 4 bit second = Position (0-15)

                uint8_t target_level = 0;
                if (cmd == WIN_OPEN)
                    target_level = 15; // เปิดสุด
                else if (cmd == WIN_CLOSE)
                    target_level = 0; // ปิดสุด
                else if (cmd == WIN_SET_POS)
                    target_level = pos_val;

                if (win_id == W_ALL)
                {
                    currentCarState.win_fl = target_level;
                    currentCarState.win_fr = target_level;
                    currentCarState.win_bl = target_level;
                    currentCarState.win_br = target_level;
                }
                else if (win_id == W_FL)
                    currentCarState.win_fl = target_level;
                else if (win_id == W_FR)
                    currentCarState.win_fr = target_level;
                else if (win_id == W_BL)
                    currentCarState.win_bl = target_level;
                else if (win_id == W_BR)
                    currentCarState.win_br = target_level;
            }

            else if (message.identifier == ID_DOMELIGHT)
            {
                uint8_t cmd = message.data[0];

                switch (cmd)
                {
                case DL_OPEN:
                    currentCarState.dl_status = true;
                    break;
                case DL_CLOSE:
                    currentCarState.dl_status = false;
                    break;
                }
            }
        }
    }
}
