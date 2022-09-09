#ifndef _DIY_BINARY_DISPLAY
#define _DIY_BINARY_DISPLAY

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

class BinaryDisplay
{
private:
    Adafruit_SSD1306 *_display;

    int _xLeft;
    int _yTop;

    byte _oldValue = 0;
    bool first = true;

    void init();

    void drawFrame();

    void drawCell(int cellIndex, bool set);

public:
    BinaryDisplay(int xLeft, int yTop, Adafruit_SSD1306 *display);

    void showValue(byte value);
};

#endif