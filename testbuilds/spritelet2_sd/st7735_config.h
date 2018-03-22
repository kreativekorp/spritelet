/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#ifndef __ST7735_CONFIG__
#define __ST7735_CONFIG__

#include "st7735_commands.h"

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 128

#define ST7735_R0_MADCTL (MADCTL_MX | MADCTL_MY | MADCTL_BGR)
#define ST7735_R0_XSTART 2
#define ST7735_R0_YSTART 3

#define ST7735_R1_MADCTL (MADCTL_MY | MADCTL_MV | MADCTL_BGR)
#define ST7735_R1_XSTART 3
#define ST7735_R1_YSTART 2

#define ST7735_R2_MADCTL (MADCTL_BGR)
#define ST7735_R2_XSTART 2
#define ST7735_R2_YSTART 1

#define ST7735_R3_MADCTL (MADCTL_MX | MADCTL_MV | MADCTL_BGR)
#define ST7735_R3_XSTART 1
#define ST7735_R3_YSTART 2

#endif
