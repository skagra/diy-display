#include "Pins.h"
#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "StatusDisplay.h"

/*
 * Operation values encoded on the address lines.
 *
 * Each value defines a particular display location/format.
 */
#define OP_VALUE_ONE 0x00 // Display on digital value 1
#define OP_VALUE_TWO 0x01 // Display on digital value 2
#define OP_MESSAGE 0x02   // Display on scrolling text line

/*
 * SSD1306 OLED Display settings
 *
 * Note: This display uses i2c and so the SDA and SCL (A4/A5 on a Nano)
 */
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

// SSD1306 OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Display code
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
    pinMode(PIN_INTERRUPT, INPUT);

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
    delay(500);
    statusDisplay->clearMessage();
    statusDisplay->setHexValueOne(0);
    statusDisplay->setHexValueTwo(0);
    statusDisplay->display();

    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), handleInterrupt, RISING);
}

// Globals to grab data/addr values
#define BUFFER_SIZE 64

byte addrBuffer[BUFFER_SIZE];
byte dataBuffer[BUFFER_SIZE];
volatile int freeBufferIndex = 0;

void handleInterrupt()
{
    if (digitalRead(PIN_ENABLE) == HIGH && freeBufferIndex < BUFFER_SIZE)
    {
        // Get the address bits
        addrBuffer[freeBufferIndex] = PINC & 0x03;

        // Get the data bits
        dataBuffer[freeBufferIndex] = (PIND >> 4) | (PINB << 4);

        freeBufferIndex++;
    }
}

// Character buffer to send message characters to display
char messageBuffer[2];

void loop()
{
    if (freeBufferIndex > 0)
    {
        noInterrupts();
        byte localAddr = *addrBuffer;
        byte localData = *dataBuffer;
        freeBufferIndex--;
        if (freeBufferIndex > 0)
        {
            memmove(dataBuffer, dataBuffer + 1, freeBufferIndex);
            memmove(addrBuffer, addrBuffer + 1, freeBufferIndex);
        }
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
            messageBuffer[0] = localData;
            messageBuffer[1] = (char)0;
            statusDisplay->addMessage(messageBuffer);
            break;
        default:
            Serial.println("ERROR: Invalid address");
            break;
        }
        statusDisplay->display();
    }
}
