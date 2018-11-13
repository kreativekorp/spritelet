#include <Arduino.h>
#include <avr/pgmspace.h>
#include "spi.h"
#include "spritelet_input.h"
#include "st7735_pins.h"
#include "st7735.h"
#include "toasters.h"

extern ST7735 tft;

#define N_FLYERS 5

static struct Flyer {
  int16_t x;
  int16_t y;
  int8_t  depth;
  uint8_t frame;
} flyer[N_FLYERS];

static const uint8_t PROGMEM toaster_mask_0[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x20, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00,
  0x00, 0x10, 0x00, 0x00, 0x00, 0x43, 0xF8, 0x00,
  0x00, 0x1F, 0x80, 0x00, 0x00, 0x7C, 0x3F, 0x80,
  0x01, 0xF1, 0xF8, 0xF0, 0x07, 0xC7, 0xC7, 0x00,
  0x0F, 0x1F, 0x08, 0x2B, 0x1E, 0x7C, 0x11, 0x00,
  0x04, 0xF0, 0x20, 0x56, 0x61, 0xE4, 0x22, 0x00,
  0x78, 0x48, 0x40, 0xA8, 0x4E, 0x10, 0x44, 0x00,
  0x53, 0x90, 0x81, 0xF8, 0x5C, 0xA0, 0x90, 0x58,
  0x7C, 0xA1, 0x07, 0x90, 0x74, 0xA1, 0x21, 0x38,
  0x7F, 0xB5, 0x0E, 0x30, 0x77, 0xB0, 0x90, 0x78,
  0x7F, 0xB4, 0x60, 0xF0, 0x77, 0xBB, 0x03, 0xE0,
  0x7F, 0xBC, 0x0F, 0xC0, 0x77, 0xBF, 0xFF, 0x00,
  0x7F, 0xBF, 0xFC, 0x00, 0x3F, 0xBF, 0xF0, 0x00,
  0x1F, 0xBF, 0xC0, 0x00, 0x07, 0xBE, 0x00, 0x00,
  0x01, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t PROGMEM toaster_mask_1[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0xF8, 0x00,
  0x00, 0x1F, 0x80, 0x00, 0x00, 0x7C, 0x3F, 0x80,
  0x01, 0xF1, 0xF8, 0x00, 0x07, 0xC7, 0xC1, 0xE0,
  0x0F, 0x1F, 0x06, 0x10, 0x1E, 0x7C, 0x00, 0x00,
  0x04, 0xF0, 0x07, 0xF0, 0x61, 0xE4, 0x18, 0x08,
  0x78, 0x48, 0x20, 0x80, 0x4E, 0x10, 0x40, 0x2B,
  0x53, 0x90, 0x81, 0x00, 0x5C, 0xA0, 0x80, 0x58,
  0x7C, 0xA1, 0x02, 0x00, 0x74, 0xA1, 0x28, 0xB8,
  0x7F, 0xB5, 0x0F, 0xF0, 0x77, 0xB0, 0x90, 0x78,
  0x7F, 0xB4, 0x60, 0xF0, 0x77, 0xBB, 0x03, 0xE0,
  0x7F, 0xBC, 0x0F, 0xC0, 0x77, 0xBF, 0xFF, 0x00,
  0x7F, 0xBF, 0xFC, 0x00, 0x3F, 0xBF, 0xF0, 0x00,
  0x1F, 0xBF, 0xC0, 0x00, 0x07, 0xBE, 0x00, 0x00,
  0x01, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t PROGMEM toaster_mask_2[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00,
  0x00, 0x1F, 0x80, 0x00, 0x00, 0x7C, 0x3F, 0x80,
  0x01, 0xF1, 0xF8, 0x00, 0x07, 0xC7, 0xC1, 0xE0,
  0x0F, 0x1F, 0x06, 0x10, 0x1E, 0x7C, 0x00, 0x00,
  0x04, 0xF0, 0x00, 0x00, 0x61, 0xE4, 0x00, 0x08,
  0x78, 0x48, 0x00, 0x10, 0x4E, 0x10, 0x00, 0x18,
  0x53, 0x90, 0x60, 0x70, 0x5C, 0xA0, 0x9F, 0xC8,
  0x7C, 0xA1, 0x04, 0x92, 0x74, 0xA1, 0x09, 0x24,
  0x7F, 0xB5, 0x02, 0x48, 0x77, 0xB0, 0x80, 0x10,
  0x7F, 0xB4, 0x41, 0x00, 0x77, 0xBB, 0x20, 0x40,
  0x7F, 0xBC, 0x10, 0x00, 0x77, 0xBF, 0xF8, 0x00,
  0x7F, 0xBF, 0xFC, 0x00, 0x3F, 0xBF, 0xF0, 0x00,
  0x1F, 0xBF, 0xC0, 0x00, 0x07, 0xBE, 0x00, 0x00,
  0x01, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t PROGMEM toast_mask[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00,
  0x00, 0x0C, 0x30, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0xC2, 0x23, 0x00, 0x07, 0x00, 0x80, 0xC0,
  0x08, 0x25, 0x50, 0x30, 0x10, 0x0B, 0xA8, 0x08,
  0x21, 0x37, 0xF5, 0x04, 0x30, 0x4A, 0xE8, 0x0C,
  0x3C, 0x15, 0x52, 0x34, 0x2B, 0x00, 0x80, 0xEC,
  0x35, 0xC2, 0x23, 0x54, 0x1A, 0xB0, 0x0E, 0xAC,
  0x0D, 0x5C, 0x15, 0x58, 0x03, 0xAB, 0xEA, 0xE0,
  0x00, 0xD5, 0x55, 0x80, 0x00, 0x3A, 0xAE, 0x00,
  0x00, 0x0D, 0x58, 0x00, 0x00, 0x03, 0xE0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t PROGMEM toaster_bitmap_0[] = {
  0x00, 0x30, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00,
  0x01, 0xD8, 0x00, 0x00, 0x03, 0x74, 0xF0, 0x00,
  0x07, 0xEF, 0xFE, 0x00, 0x07, 0xBC, 0x07, 0x80,
  0x0F, 0xE0, 0x7F, 0xE0, 0x0F, 0x83, 0xC0, 0x70,
  0x1E, 0x0E, 0x07, 0x08, 0x18, 0x38, 0x38, 0xFF,
  0x30, 0xE0, 0xF7, 0xD4, 0x61, 0x83, 0xEE, 0xFF,
  0x7B, 0x0F, 0xDF, 0xA8, 0x9E, 0x1B, 0xDD, 0xFE,
  0x87, 0xB7, 0xBF, 0x50, 0xB1, 0xEF, 0xBB, 0xF8,
  0xAC, 0x6F, 0x7E, 0x00, 0xA3, 0x5F, 0x6F, 0xA0,
  0x83, 0x5E, 0xF8, 0x68, 0x8B, 0x5E, 0xDE, 0xC0,
  0x80, 0x4A, 0xF1, 0xC8, 0x88, 0x4F, 0x6F, 0x80,
  0x80, 0x4B, 0x9F, 0x08, 0x88, 0x44, 0xFC, 0x10,
  0x80, 0x43, 0xF0, 0x20, 0x88, 0x40, 0x00, 0xC0,
  0x80, 0x40, 0x03, 0x00, 0x40, 0x40, 0x0C, 0x00,
  0x20, 0x40, 0x30, 0x00, 0x18, 0x41, 0xC0, 0x00,
  0x06, 0x4E, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00
};

static const uint8_t PROGMEM toaster_bitmap_1[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xF0, 0x00,
  0x00, 0xF7, 0xFE, 0x00, 0x01, 0xBC, 0x07, 0x80,
  0x03, 0xE0, 0x7F, 0xE0, 0x07, 0x83, 0xC0, 0x70,
  0x0E, 0x0E, 0x07, 0xF8, 0x18, 0x38, 0x3E, 0x18,
  0x30, 0xE0, 0xF9, 0xE8, 0x61, 0x83, 0xFF, 0xF8,
  0x7B, 0x0F, 0xF8, 0x08, 0x9E, 0x1B, 0xE7, 0xF0,
  0x87, 0xB7, 0xDF, 0x7F, 0xB1, 0xEF, 0xBF, 0xD4,
  0xAC, 0x6F, 0x7E, 0xFF, 0xA3, 0x5F, 0x7F, 0xA6,
  0x83, 0x5E, 0xFD, 0xF8, 0x8B, 0x5E, 0xD7, 0x40,
  0x80, 0x4A, 0xF0, 0x08, 0x88, 0x4F, 0x6F, 0x80,
  0x80, 0x4B, 0x9F, 0x08, 0x88, 0x44, 0xFC, 0x10,
  0x80, 0x43, 0xF0, 0x20, 0x88, 0x40, 0x00, 0xC0,
  0x80, 0x40, 0x03, 0x00, 0x40, 0x40, 0x0C, 0x00,
  0x20, 0x40, 0x30, 0x00, 0x18, 0x41, 0xC0, 0x00,
  0x06, 0x4E, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00
};

static const uint8_t PROGMEM toaster_bitmap_2[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00,
  0x00, 0x07, 0xFE, 0x00, 0x00, 0x3C, 0x07, 0x80,
  0x00, 0xE0, 0x7F, 0xE0, 0x03, 0x83, 0xC0, 0x70,
  0x0E, 0x0E, 0x07, 0xF8, 0x18, 0x38, 0x3E, 0x18,
  0x30, 0xE0, 0xF9, 0xE8, 0x61, 0x83, 0xFF, 0xF8,
  0x7B, 0x0F, 0xFF, 0xF8, 0x9E, 0x1B, 0xFF, 0xF0,
  0x87, 0xB7, 0xFF, 0xE8, 0xB1, 0xEF, 0xFF, 0xE0,
  0xAC, 0x6F, 0x9F, 0x88, 0xA3, 0x5F, 0x60, 0x36,
  0x83, 0x5E, 0xFB, 0x6D, 0x8B, 0x5E, 0xF6, 0xDB,
  0x80, 0x4A, 0xFD, 0xB6, 0x88, 0x4F, 0x7F, 0xEE,
  0x80, 0x4B, 0xBE, 0xFC, 0x88, 0x44, 0xDF, 0xBC,
  0x80, 0x43, 0xEF, 0xF8, 0x88, 0x40, 0x07, 0xF0,
  0x80, 0x40, 0x03, 0xE0, 0x40, 0x40, 0x0C, 0xC0,
  0x20, 0x40, 0x30, 0x00, 0x18, 0x41, 0xC0, 0x00,
  0x06, 0x4E, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00
};

static const uint8_t PROGMEM toast_bitmap[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xC0, 0x00, 0x00, 0x0F, 0xF0, 0x00,
  0x00, 0x3D, 0xDC, 0x00, 0x00, 0xFF, 0x7F, 0x00,
  0x07, 0xDA, 0xAF, 0xC0, 0x0F, 0xF4, 0x57, 0xF0,
  0x1E, 0xC8, 0x0A, 0xF8, 0x0F, 0xB5, 0x17, 0xF0,
  0x03, 0xEA, 0xAD, 0xC8, 0x14, 0xFF, 0x7F, 0x10,
  0x0A, 0x3D, 0xDC, 0xA8, 0x05, 0x4F, 0xF1, 0x50,
  0x02, 0xA3, 0xEA, 0xA0, 0x00, 0x54, 0x15, 0x00,
  0x00, 0x2A, 0xAA, 0x00, 0x00, 0x05, 0x50, 0x00,
  0x00, 0x02, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t * const toaster_mask[] PROGMEM = {
  toaster_mask_0,
  toaster_mask_1,
  toaster_mask_2,
  toaster_mask_1,
  toast_mask
};

static const uint8_t * const toaster_bitmap[] PROGMEM = {
  toaster_bitmap_0,
  toaster_bitmap_1,
  toaster_bitmap_2,
  toaster_bitmap_1,
  toast_bitmap
};

uint8_t toasters_setup(void) {
  uint8_t i;
  for (i = 0; i < N_FLYERS; i++) {
    flyer[i].x     = (random(160) - 32) << 4;
    flyer[i].y     = (random(160) - 32) << 4;
    flyer[i].depth = random(16) + 10;
    flyer[i].frame = random(4) ? random(4) : 4;
  }
  return 1;
}

uint8_t toasters_loop(void) {
  uint32_t t = millis();
  uint8_t line[128];
  uint8_t y, x, i, f;
  int16_t fy, fx;
  uint8_t * mask;
  uint8_t * bitmap;
  uint8_t mbi, mbm;

  tft.setAddrWindow(0, 0, 127, 127);
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW );
  for (y = 0; y < 128; y++) {
    for (x = 0; x < 128; x++) {
      line[x] = 0;
    }
    for (i = 0; i < N_FLYERS; i++) {
      fy = flyer[i].y >> 4;
      if ((y >= fy) && (y < fy + 32)) {
        f = flyer[i].frame;
        mask = (uint8_t *)pgm_read_word(&toaster_mask[f]);
        bitmap = (uint8_t *)pgm_read_word(&toaster_bitmap[f]);
        mbi = ((y - fy) << 2);
        mbm = 0x80;
        for (fx = flyer[i].x >> 4, x = 0; x < 32; x++, fx++) {
          if (fx >= 0 && fx < 128) {
            if (pgm_read_byte(&mask[mbi]) & mbm) line[fx] = 0;
            if (pgm_read_byte(&bitmap[mbi]) & mbm) line[fx] = -1;
          }
          mbm >>= 1;
          if (!mbm) {
            mbm = 0x80;
            mbi++;
          }
        }
      }
    }
    for (x = 0; x < 128; x++) {
      SPI_write(line[x]);
      SPI_write(line[x]);
    }
  }
  digitalWrite(TFT_CS, HIGH);

  for (i = 0; i < N_FLYERS; i++) {
    if (flyer[i].frame < 4) flyer[i].frame = ((flyer[i].frame + 1) & 3);
    flyer[i].x -= flyer[i].depth;
    flyer[i].x -= flyer[i].depth;
    flyer[i].y += flyer[i].depth;
    if ((flyer[i].y >= 2048) || (flyer[i].x <= -512)) {
      if (random(2)) {
        flyer[i].x = random(128) << 4;
        flyer[i].y = -512;
      } else {
        flyer[i].x = 2048;
        flyer[i].y = (random(160) - 32) << 4;
      }
      flyer[i].depth = random(16) + 10;
      flyer[i].frame = random(4) ? random(4) : 4;
    }
  }

  do { 
    if (input_get() == INPUT_CTR) {
      tft.fillScreen(0);
      delay(50);
      while (input_get());
      delay(50);
      return 0;
    }
  } while (millis() - t < 30);
  return 1;
}
