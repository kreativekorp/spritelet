/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#ifndef __ST7735_INIT__
#define __ST7735_INIT__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "st7735_commands.h"
#include "st7735_config.h"

const uint8_t PROGMEM st7735_init_commands[] = {
  ST7735_SWRESET,   0,                   // Software reset
  ST7735_DELAY  , 150,                   // 150 ms delay
  ST7735_SLPOUT ,   0,                   // Out of sleep mode
  ST7735_DELAY  , 250,                   // 500 ms delay
  ST7735_DELAY  , 250,
  ST7735_FRMCTR1,   3, 0x01, 0x2C, 0x2D, // Frame rate ctrl, normal mode = fosc/(1x2+40) * (LINE+2C+2D)
  ST7735_FRMCTR2,   3, 0x01, 0x2C, 0x2D, // Frame rate ctrl, idle mode = fosc/(1x2+40) * (LINE+2C+2D)
  ST7735_FRMCTR3,   6, 0x01, 0x2C, 0x2D, // Frame rate ctrl, dot inversion mode
                       0x01, 0x2C, 0x2D, // Frame rate ctrl, line inversion mode
  ST7735_INVCTR ,   1, 0x07,             // Display inversion control = no inversion
  ST7735_PWCTR1 ,   3, 0xA2, 0x02, 0x84, // Power control, -4.6V, AUTO mode
  ST7735_PWCTR2 ,   1, 0xC5,             // Power control, VGH25 = 2.4C, VGSEL = -10, VGH = 3 * AVDD
  ST7735_PWCTR3 ,   2, 0x0A, 0x00,       // Power control, opamp current small, boost frequency
  ST7735_PWCTR4 ,   2, 0x8A, 0x2A,       // Power control, BCLK/2, opamp current small and medium low
  ST7735_PWCTR5 ,   2, 0x8A, 0xEE,       // Power control
  ST7735_VMCTR1 ,   1, 0x0E,             // Power control
  ST7735_INVOFF ,   0,                   // Don't invert display
  ST7735_COLMOD ,   1, 0x05,             // Set color mode, 16-bit color

  // Memory address control, column address, row address
  ST7735_MADCTL ,   1, ST7735_R0_MADCTL,
  ST7735_CASET  ,   4, 0x00, ST7735_R0_XSTART,
                       0x00, (ST7735_R0_XSTART + ST7735_TFTWIDTH - 1),
  ST7735_RASET  ,   4, 0x00, ST7735_R0_YSTART,
                       0x00, (ST7735_R0_YSTART + ST7735_TFTHEIGHT - 1),

  // Gamma correction
  ST7735_GMCTRP1,  16, 0x02, 0x1C, 0x07, 0x12,
                       0x37, 0x32, 0x29, 0x2D,
                       0x29, 0x25, 0x2B, 0x39,
                       0x00, 0x01, 0x03, 0x10,
  ST7735_GMCTRN1,  16, 0x03, 0x1D, 0x07, 0x06,
                       0x2E, 0x2C, 0x29, 0x2D,
                       0x2E, 0x2E, 0x37, 0x3F,
                       0x00, 0x00, 0x02, 0x10,

  ST7735_NORON  ,   0,                   // Normal display on
  ST7735_DELAY  ,  10,                   // 10 ms delay
  ST7735_DISPON ,   0,                   // Main screen turn on
  ST7735_DELAY  , 100,                   // 100 ms delay
  ST7735_EOF                             // End of command list
};

#endif
