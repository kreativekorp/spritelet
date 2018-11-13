/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#ifndef __ST7735_PINS__
#define __ST7735_PINS__

#include <Arduino.h>

// Chip select line for TFT display.
#define TFT_CS        10
#define TFT_CS_DDR    DDRB
#define TFT_CS_PORT   PORTB
#define TFT_CS_PIN    PINB
#define TFT_CS_MASK   0x04

// Data/command line for TFT display.
#define TFT_DC        8
#define TFT_DC_DDR    DDRB
#define TFT_DC_PORT   PORTB
#define TFT_DC_PIN    PINB
#define TFT_DC_MASK   0x01

// Reset line for TFT display.
#define TFT_RST       6
#define TFT_RST_DDR   DDRD
#define TFT_RST_PORT  PORTD
#define TFT_RST_PIN   PIND
#define TFT_RST_MASK  0x40

// Backlight pin for TFT display.
#define TFT_LITE      9
#define TFT_LITE_DDR  DDRB
#define TFT_LITE_PORT PORTB
#define TFT_LITE_PIN  PINB
#define TFT_LITE_MASK 0x02

#endif
