#include <Arduino.h>
#include "task\TFT\TFT.h"
#include "task\CAN\CAN.h"

volatile CarData currentCarState;

void setup()
{
  Serial.begin(115200);
  Can_init();
  TFT_init();
  xTaskCreate(Rx_Can, "Recive_Can", 4096, NULL, 2, NULL);
  //xTaskCreate(Display, "Display_TFT", 4096, NULL, 1, NULL);
}

void loop()
{
  vTaskDelete(NULL);
}
