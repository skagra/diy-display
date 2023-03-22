#include "Scroller.h"

Scroller::Scroller(int width, Adafruit_SSD1306 *display)
{
    _display = display;
    _bufferSize = width;

    _buffer = (char *)malloc(_bufferSize);
    memset(_buffer, (byte)' ', _bufferSize);
    _buffer[_bufferSize - 1] = (char)0;
}

void Scroller::addCharToBuffer(const char character)
{
    if (_bufferIndex < _bufferSize - 1)
    {
        _buffer[_bufferIndex] = character;
        _bufferIndex++;
    }
    else
    {
        memmove(_buffer, _buffer + 1, _bufferSize - 1);
        _buffer[_bufferSize - 1] = (char)0;
        _buffer[_bufferIndex - 2] = character;
    }
}

void Scroller::clear()
{
    memset(_buffer, (byte)' ', _bufferSize - 1);
    _display->fillRect(0, 40, 128, 20, SSD1306_BLACK);
    _bufferIndex = 0;
}

void Scroller::addChar(const char character)
{
    addCharToBuffer(character);

    _display->fillRect(0, 40, 128, 20, SSD1306_BLACK);
    _display->setCursor(0, 40);
    _display->print(_buffer);
}