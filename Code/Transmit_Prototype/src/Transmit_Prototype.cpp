#include <Arduino.h>
#include "driver/twai.h"

// กำหนดขา Pin สำหรับ ESP32
#define TX_PIN (gpio_num_t)25
#define RX_PIN (gpio_num_t)26

void setup()
{
    Serial.begin(115200);

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // 2. ติดตั้งและเริ่มทำงาน
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK && twai_start() == ESP_OK)
    {
        Serial.println("CAN Test Started... Sending every 100ms");
    }
    else
    {
        Serial.println("CAN Init Failed!");
        while (1)
            ;
    }
}

void loop()
{

    twai_message_t msg1;
    msg1.identifier = 0x110;
    msg1.data_length_code = 2;
    msg1.data[0] = 0x01;
    msg1.data[1] = 0xFF;
    msg1.extd = 0;
    msg1.rtr = 0;
    msg1.ss = 0;

    twai_message_t msg2 = msg1;
    msg2.identifier = 0x120;
    msg2.data[1] = 0x5A;

    // ส่งออกไป
    if (twai_transmit(&msg1, pdMS_TO_TICKS(10)) == ESP_OK)
    {
        Serial.print("Sent 0x110... ");
    }
    else
    {
        Serial.print("Send 0x110 Failed (Check Wiring/ACK)... ");
    }

    if (twai_transmit(&msg2, pdMS_TO_TICKS(10)) == ESP_OK)
    {
        Serial.println("Sent 0x120");
    }
    else
    {
        Serial.println("Sent 0x120 Failed");
    }

    delay(100);
}