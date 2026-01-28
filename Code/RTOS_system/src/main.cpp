#include <Arduino.h>
#include <SPI.h>
#include "task\TFT\TFT.h"
#include "task\CAN\CAN.h"

void setup()
{
  Serial.begin(115200);
  // Can_init();
  TFT_init();
  // xTaskCreate(Rx_Can, "Recive_Can", 4096, NULL, 1, NULL);
  xTaskCreate(Display, "Display_TFT", 4096, NULL, 1, NULL);
}

void loop()
{
  vTaskDelete(NULL);
}
