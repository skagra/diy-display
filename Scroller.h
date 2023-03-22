#ifndef _SCROLLER_DOT_H_
#define _SCROLLER_DOT_H_

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

class Scroller
{
private:
    Adafruit_SSD1306 *_display;
    char *_buffer;
    int _bufferIndex = 0;
    int _bufferSize;

    void addCharToBuffer(const char character);

public:
    Scroller(int width, Adafruit_SSD1306 *display);

    void addChar(const char character);

    void clear();
};

#endif