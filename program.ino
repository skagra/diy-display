#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

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

// SDA 23 SDA  A4
// SCL 24 /SCL A5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

    // for (int pin = PIN_DATA_0; pin <= PIN_DATA_7; pin++)
    // {
    //     pinMode(pin, OUTPUT);
    // }
    // for (int pin = PIN_ADDR_A0; pin <= PIN_ADDR_A3; pin++)
    // {
    //     pinMode(pin, OUTPUT);
    // }
    // //  pinMode(PIN_CLOCK, OUTPUT);
    // pinMode(PIN_ENABLE, OUTPUT);

    // pinMode(PIN_CLOCK, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_CLOCK), handleClock, RISING);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.display();
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

void loop()
{

    if (dataExists)
    {
        //  noInterrupts();
        dataExists = false;
        switch (data)
        {
        default:
            Serial.println("INT");
            display.clearDisplay();
            display.setCursor(1, 1);
            display.println(addr);
            display.println(data);
            display.display();
            break;
        }
        // interrupts();
    }
}
