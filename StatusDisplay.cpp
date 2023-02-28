#include "StatusDisplay.h"

#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
// #include "Fonts/FreeSans9pt7b.h"

const int MARGIN = 4;

#define ERROR_PERSISTENCE_MILLIS 300

StatusDisplay::StatusDisplay(Adafruit_SSD1306 *display)
{
    _display = display;

    _displayWidth = display->width();
    _displayHeight = display->height();

    init();
}

void StatusDisplay::init()
{
    int16_t x1, y1;
    uint16_t w, h;

    _display->clearDisplay();
    _display->setTextWrap(false);

    _display->setTextColor(SSD1306_WHITE);
    //   _display->setFont(&FreeSans9pt7b);
    //_fontHeight = FreeSans9pt7b.yAdvance;

    _valueOne = new ValueDisplay(0, 0, _display);
    _valueTwo = new ValueDisplay(_displayWidth / 2, 0, _display);

    _scroller = new Scroller(26, _display);

    _display->display();
}

void StatusDisplay::setHexValue(int x, int y, byte value)
{
    _display->setCursor(x, y);
    _display->print(value);
}

void StatusDisplay::setHexValueOne(byte value)
{
    _valueOne->showValue(value);
}

void StatusDisplay::setHexValueTwo(byte value)
{
    _valueTwo->showValue(value);
}

void StatusDisplay::addMessage(const char *message)
{
    while ((*message) != (char)0)
    {
        _scroller->addChar(*message);
        message++;
    }
}

void StatusDisplay::display()
{
    _display->display();
}