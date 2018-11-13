/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#ifndef __ST7735_PINS__
#define __ST7735_PINS__

#include <Arduino.h>

// Chip select line for TFT display.
#define TFT_CS        9
#define TFT_CS_DDR    DDRB
#define TFT_CS_PORT   PORTB
#define TFT_CS_PIN    PINB
#define TFT_CS_MASK   0x20

// Data/command line for TFT display.
#define TFT_DC        11
#define TFT_DC_DDR    DDRB
#define TFT_DC_PORT   PORTB
#define TFT_DC_PIN    PINB
#define TFT_DC_MASK   0x80

// Reset line for TFT display.
#define TFT_RST       10
#define TFT_RST_DDR   DDRB
#define TFT_RST_PORT  PORTB
#define TFT_RST_PIN   PINB
#define TFT_RST_MASK  0x40

// Backlight pin for TFT display.
#define TFT_LITE      13
#define TFT_LITE_DDR  DDRC
#define TFT_LITE_PORT PORTC
#define TFT_LITE_PIN  PINC
#define TFT_LITE_MASK 0x80

#endif
