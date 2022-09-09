#include "ValueDisplay.h"

#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define CHAR_GAP 3

void ValueDisplay::init()
{
   showValue(0);
}

void hexString(byte value, char *buffer)
{
   sprintf(buffer, "H:%02X", value);
   buffer[4] = (byte)0;
}

void decString(byte value, char *buffer)
{
   sprintf(buffer, "D:%03i", value);
   buffer[5] = (byte)0;
}

void ValueDisplay::drawHex(byte value)
{
   _display->fillRect(_xLeft, _yTop, 4 * CHAR_WIDTH, CHAR_HEIGHT, SSD1306_BLACK);

   hexString(value, _stringBuffer);
   _display->setCursor(_xLeft, _yTop);
   _display->print(_stringBuffer);
}

void ValueDisplay::drawDec(byte value)
{
   _display->fillRect(_xLeft + 4 * CHAR_WIDTH + CHAR_GAP, _yTop, 30, 7, SSD1306_BLACK);

   decString(value, _stringBuffer);
   _display->setCursor(_xLeft + 4 * CHAR_WIDTH + CHAR_GAP, _yTop);
   _display->print(_stringBuffer);
}

void ValueDisplay::drawBin(byte value)
{
   _binaryDisplay->showValue(value);
}

ValueDisplay::ValueDisplay(int xLeft, int yTop, Adafruit_SSD1306 *display)
{
   _display = display;

   _xLeft = xLeft;
   _yTop = yTop;

   _display->setTextWrap(false);
   _display->setTextColor(SSD1306_WHITE);

   _binaryDisplay = new BinaryDisplay(xLeft, _yTop + 12, display);

   init();
}

void ValueDisplay::showValue(byte value)
{
   if (value != _oldValue || first)
   {
      drawHex(value);
      drawDec(value);
      drawBin(value);

      _display->display();

      _oldValue = value;
      first = false;
   }
}
