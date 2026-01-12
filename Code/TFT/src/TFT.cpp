#include <TFT_eSPI.h>
#include "Frame_1.h"
#include "Frame_2.h"
#include "image.h"

TFT_eSPI tft;

void setup()
{
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(1);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(
      0, 0,
      IMAGE_WIDTH,
      IMAGE_HEIGHT,
      image);
}

void loop()
{
  
}

