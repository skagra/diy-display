#ifndef _DIY_STATUS_DISPLAY
#define _DIY_STATUS_DISPLAY

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "ValueDisplay.h"
#include "Scroller.h"

class StatusDisplay
{
private:
    Adafruit_SSD1306 *_display;
    int16_t _displayWidth;
    int16_t _displayHeight;

    int _fontHeight;
    int _fontBaseOffset;

    void init();

    void setHexValue(int x, int y, byte value);

    ValueDisplay *_valueOne;
    ValueDisplay *_valueTwo;
    Scroller *_scroller;

public:
    StatusDisplay(Adafruit_SSD1306 *display);

    void setHexValueOne(byte value);

    void setHexValueTwo(byte value);

    void addMessage(const char *message);
};

#endif