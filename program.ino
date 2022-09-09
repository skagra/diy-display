#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "StatusDisplay.h"

#define PIN_DATA_0 4  // PIN 7
#define PIN_DATA_1 5  // PIN 8
#define PIN_DATA_2 6  // PIN 9
#define PIN_DATA_3 7  // PIN 10
#define PIN_DATA_4 8  // PIN 11
#define PIN_DATA_5 9  // PIN 12
#define PIN_DATA_6 10 // PIN 13
#define PIN_DATA_7 11 // PIN 14

#define PIN_ADDR_A0 PIN_A0 // PIN 10
#define PIN_ADDR_A1 PIN_A1 // PIN 20
#define PIN_ADDR_A2 PIN_A2 // PIN 21
#define PIN_ADDR_A3 PIN_A3 // PIN 22

#define PIN_ENABLE 3 // PIN 6
#define PIN_CLOCK 2  // PIN 5

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

// SDA 23 A4
// SCL 24 A5

#define OP_VALUE_ONE 0x00
#define OP_VALUE_TWO 0x01
#define OP_MESSAGE 0x02

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
            ;
    }

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
        addr = (digitalRead(PIN_ADDR_A0) == HIGH) |
               (digitalRead(PIN_ADDR_A1) == HIGH) << 1 |
               (digitalRead(PIN_ADDR_A2) == HIGH) << 2 |
               (digitalRead(PIN_ADDR_A3) == HIGH) << 3;

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
        switch (data)
        {
        case (OP_VALUE_ONE):
            statusDisplay->setHexValueOne(data);
            break;
        case (OP_VALUE_TWO):
            statusDisplay->setHexValueTwo(data);
            break;
        case (OP_MESSAGE):
            buffer[0] = data;
            buffer[1] = (char)0;
            statusDisplay->addMessage(buffer);
            break;
        default:
            Serial.println("ERROR: Invalid address");
            break;
        }
    }
}
