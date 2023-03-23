#ifndef _PIN_DOT_H_
#define _PIN_DOT_H_

/*
 * DATA
 *
 * Byte to display:
 *
 * - LSB is on lowest numbered pin
 * - The byte is interpreted either as a character or a number
 *   depending on the operation selected by the ADDR pins.
 */
#define PIN_DATA_0 4  // PIN 7
#define PIN_DATA_1 5  // PIN 8
#define PIN_DATA_2 6  // PIN 9
#define PIN_DATA_3 7  // PIN 10
#define PIN_DATA_4 8  // PIN 11
#define PIN_DATA_5 9  // PIN 12
#define PIN_DATA_6 10 // PIN 13
#define PIN_DATA_7 11 // PIN 14

/*
 * ADDR
 *
 * The value selects the operation (below).
 *
 * - LSB is on lowest numbered pin
 */
#define PIN_ADDR_A0 PIN_A0 // PIN 10
#define PIN_ADDR_A1 PIN_A1 // PIN 20

/*
 * Interrupt
 *
 * Values are read from the DATA and ADDR pins on a rising edge on the interrupt line.
 */
#define PIN_INTERRUPT 2 // PIN 5

/*
 * ENABLE
 *
 * Values will only be read from DATA and ADDR when ENABLE is high.
 */
#define PIN_ENABLE 3 // PIN 6

#endif