#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "StatusDisplay.h"

#define PIN_DATA_0 4  // PIN
#define PIN_DATA_1 5  // PIN
#define PIN_DATA_2 6  // PIN
#define PIN_DATA_3 7  // PIN
#define PIN_DATA_4 8  // PIN
#define PIN_DATA_5 9  // PIN
#define PIN_DATA_6 10 // PIN
#define PIN_DATA_7 11 // PIN

#define PIN_ADDR_0 PIN_A0 // PIN
#define PIN_ADDR_1 PIN_A1 // PIN
#define PIN_ADDR_2 PIN_A2 // PIN
#define PIN_ADDR_3 PIN_A3 // PIN

#define PIN_ENABLE 3 // PIN
#define PIN_CLOCK 2  // PIN

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define OP_ADDR_VALUE_ONE 0x00
#define OP_ADDR_VALUE_TWO 0x01
#define OP_ADDR_MESSAGE 0x02

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
StatusDisplay *statusDisplay;

void setup()
{
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
        {
            delay(1000);
        }
    }

    for (int addrPin = PIN_ADDR_0; addrPin <= PIN_ADDR_3; addrPin++)
    {
        pinMode(addrPin, INPUT);
    }

    for (int dataPin = PIN_DATA_0; dataPin <= PIN_DATA_7; dataPin++)
    {
        pinMode(dataPin, INPUT);
    }

    pinMode(PIN_ENABLE, INPUT);
    pinMode(PIN_CLOCK, INPUT);

    statusDisplay = new StatusDisplay(&display);

    attachInterrupt(digitalPinToInterrupt(PIN_CLOCK), handleClock, RISING);
}

volatile byte addr;
volatile byte data;
volatile bool dataExists = false;

void handleClock()
{
    if (digitalRead(PIN_ENABLE) == HIGH)
    {
        // Get the address bits
        addr = (digitalRead(PIN_ADDR_0) == HIGH) |
               (digitalRead(PIN_ADDR_1) == HIGH) << 1 |
               (digitalRead(PIN_ADDR_2) == HIGH) << 2 |
               (digitalRead(PIN_ADDR_3) == HIGH) << 3;

        // Get the data bits
        data = (digitalRead(PIN_DATA_0) == HIGH) |
               (digitalRead(PIN_DATA_1) == HIGH) << 1 |
               (digitalRead(PIN_DATA_2) == HIGH) << 2 |
               (digitalRead(PIN_DATA_3) == HIGH) << 3 |
               (digitalRead(PIN_DATA_4) == HIGH) << 4 |
               (digitalRead(PIN_DATA_5) == HIGH) << 5 |
               (digitalRead(PIN_DATA_6) == HIGH) << 6 |
               (digitalRead(PIN_DATA_7) == HIGH) << 7;

        dataExists = true;
    }
}

char buffer[2];

void loop()
{
    if (dataExists)
    {
        dataExists = false;
        switch (addr)
        {
        case (OP_ADDR_VALUE_ONE):
            statusDisplay->setHexValueOne(data);
            break;
        case (OP_ADDR_VALUE_TWO):
            statusDisplay->setHexValueTwo(data);
            break;
        case (OP_ADDR_MESSAGE):
            buffer[0] = data;
            buffer[1] = (char)0;
            statusDisplay->addMessage(buffer);
            break;
        default:
            Serial.print("ERROR: Invalid address: ");
            Serial.println(addr, HEX);
            break;
        }
    }
}
