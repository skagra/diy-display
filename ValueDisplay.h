#ifndef _DIY_VALUE_DISPLAY
#define _DIY_VALUE_DISPLAY

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "BinaryDisplay.h"

class ValueDisplay
{
private:
    Adafruit_SSD1306 *_display;

    BinaryDisplay *_binaryDisplay;

    int _xLeft;
    int _yTop;

    byte _oldValue = 0;
    bool first = true;

    char _stringBuffer[6];

    void init();
    void drawHex(byte value);
    void drawDec(byte value);
    void drawBin(byte value);

public:
    ValueDisplay(int xLeft, int yTop, Adafruit_SSD1306 *display);

    void showValue(byte value);
};

#endif