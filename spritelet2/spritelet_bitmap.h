#ifndef __SPRITELET_BITMAP__
#define __SPRITELET_BITMAP__

#include <Arduino.h>

#define SMF_OK    0x00
#define SMF_ERROR 0xFF

uint8_t tft_drawSMF(char * path, uint8_t (*callback)(void));
uint8_t tft_drawGCI(uint32_t sect, uint16_t offs, int16_t x, int16_t y, uint8_t (*callback)(void));

#endif
