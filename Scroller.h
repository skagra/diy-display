#ifndef _SCROLLER_DOT_H_
#define _SCROLLER_DOT_H_

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

class Scroller
{
private:
    Adafruit_SSD1306 *_display;

public:
    Scroller(int width, Adafruit_SSD1306 *display);

    void addChar(const char character);
};

#endif