/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#ifndef __ST7735_PINS__
#define __ST7735_PINS__

#include <Arduino.h>

// Chip select line for TFT display.
#define TFT_CS    10

// Data/command line for TFT display.
#define TFT_DC    8

// Reset line for TFT display.
#define TFT_RST   9

// Backlight pin for TFT display.
#define TFT_LITE  5

#endif
