#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

void setup()
{
    tft.setSPISpeed(125000); // ช้าที่สุด นิ่งสุด
    tft.begin();

    tft.setRotation(0); 
    tft.fillScreen(ILI9341_GREEN);
     tft.setRotation(2); 
    tft.fillScreen(ILI9341_GREEN);
}

void loop() {}
