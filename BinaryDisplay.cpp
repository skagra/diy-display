#include "BinaryDisplay.h"

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

const int CELL_SIZE = 7;

BinaryDisplay::BinaryDisplay(int xLeft, int yTop, Adafruit_SSD1306 *display)
{
   _display = display;
   _xLeft = xLeft;
   _yTop = yTop;

   init();
}

void BinaryDisplay::init()
{
   drawFrame();
   showValue(0);
}

void BinaryDisplay::drawFrame()
{
   for (int cell = 0; cell < 8; cell++)
   {
      _display->drawRect(_xLeft + CELL_SIZE * cell, _yTop, CELL_SIZE + 1, CELL_SIZE + 1, SSD1306_WHITE);
   }
   _display->display();
}

void BinaryDisplay::drawCell(int cellIndex, bool set)
{
   unsigned int color = (set) ? SSD1306_WHITE : SSD1306_BLACK;
   _display->fillRect(_xLeft + CELL_SIZE * cellIndex + 2, _yTop + 2, CELL_SIZE - 3, CELL_SIZE - 3, color);
}

void BinaryDisplay::showValue(byte value)
{
   if (value != _oldValue || first)
   {
      int bitMask = 128;

      for (int cell = 0; cell < 8; cell++)
      {
         bool set = value & bitMask;
         drawCell(cell, set);
         bitMask /= 2;
      }
      _display->display();

      first = false;
      _oldValue = value;
   }
}
