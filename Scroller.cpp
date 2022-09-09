#include "Scroller.h"

char *bbuffer;

// int bufferLocation = 0;
int bufferSize;
int charWidth = 6;
int scrollerY;

Scroller::Scroller(int width, Adafruit_SSD1306 *display)
{
    _display = display;

    bufferSize = width;
    bbuffer = (char *)malloc(bufferSize);
}

void Scroller::addChar(const char character)
{
    bbuffer[bufferSize - 1] = character;
}