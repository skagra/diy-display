#include <arduino.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

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

#define PIN_ENABLE 3 // PIN 6
#define PIN_CLOCK 2  // PIN 5

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define OP_VALUE_ONE 0x00
#define OP_VALUE_TWO 0x01
#define OP_MESSAGE 0x02

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
StatusDisplay *statusDisplay;

void setup()
{
    Serial.begin(9600);

    pinMode(PIN_ADDR_A0, INPUT);
    pinMode(PIN_ADDR_A1, INPUT);

    pinMode(PIN_DATA_0, INPUT);
    pinMode(PIN_DATA_1, INPUT);
    pinMode(PIN_DATA_2, INPUT);
    pinMode(PIN_DATA_3, INPUT);
    pinMode(PIN_DATA_4, INPUT);
    pinMode(PIN_DATA_5, INPUT);
    pinMode(PIN_DATA_6, INPUT);
    pinMode(PIN_DATA_7, INPUT);

    pinMode(PIN_ENABLE, INPUT);
    pinMode(PIN_CLOCK, INPUT);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    statusDisplay = new StatusDisplay(&display);

    statusDisplay->setHexValueOne(0xFF);
    statusDisplay->setHexValueTwo(0xFF);
    statusDisplay->addMessage("DIY CPU");
    statusDisplay->display();
    delay(500);
    statusDisplay->addMessage("...READY!");
    statusDisplay->display();
    delay(1000);
    statusDisplay->clearMessage();
    statusDisplay->setHexValueOne(0);
    statusDisplay->setHexValueTwo(0);
    statusDisplay->display();

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
        addr = PINC & 0x03;

        // Get the data bits
        data = (PIND >> 4) | (PINB << 4);

        dataExists = true;
    }
}

char buffer[2];

void loop()
{
    if (dataExists)
    {
        noInterrupts();
        byte localAddr = addr;
        byte localData = data;
        dataExists = false;
        interrupts();

        switch (localAddr)
        {
        case (OP_VALUE_ONE):
            statusDisplay->setHexValueOne(localData);
            break;
        case (OP_VALUE_TWO):
            statusDisplay->setHexValueTwo(localData);
            break;
        case (OP_MESSAGE):
            buffer[0] = localData;
            buffer[1] = (char)0;
            statusDisplay->addMessage(buffer);
            break;
        default:
            Serial.println("ERROR: Invalid address");
            break;
        }
        statusDisplay->display();
    }
}
