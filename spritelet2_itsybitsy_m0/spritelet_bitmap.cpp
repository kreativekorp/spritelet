#include <Arduino.h>
#include "picofs.h"
#include "spi.h"
#include "spritelet_bitmap.h"
#include "st7735_pins.h"
#include "st7735.h"

extern ST7735 tft;
extern FATFS fs;

static uint16_t tft_blit(uint16_t ptr, int16_t x, int16_t y, int16_t w, int16_t h) {
  uint32_t n;
  uint16_t rem;
  n   = w;
  n  *= h;
  n <<= 1;
  if (n) {
    tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
    rem = 512 - ptr;
    if (rem > n) rem = n;
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW );
    SPI_writeBlock(fs.buf + ptr, rem);
    digitalWrite(TFT_CS, HIGH);
    n -= rem;
    ptr += rem;
    while (n && !fs.read()) {
      rem = 512;
      if (rem > n) rem = n;
      digitalWrite(TFT_DC, HIGH);
      digitalWrite(TFT_CS, LOW );
      SPI_writeBlock(fs.buf, rem);
      digitalWrite(TFT_CS, HIGH);
      n -= rem;
      ptr = rem;
    }
  }
  return ptr;
}

uint8_t tft_drawSMF(char * path, uint8_t (*callback)(void)) {
  uint32_t time = millis();
  int16_t bx = 0, by = 0;
  int16_t x, y, w, h;
  uint32_t n;
  uint16_t clr;
  uint16_t ptr;
  uint8_t cmd;
  if (fs.open(path)) return SMF_ERROR;
  while (!fs.read()) {
    ptr = 0;
    while (ptr < 512) {
      cmd = fs.buf[ptr++];
      if (cmd == 'P') {
        x = fs.buf[ptr++];
        y = fs.buf[ptr++];
        w = fs.buf[ptr++];
        h = fs.buf[ptr++];
        ptr = tft_blit(ptr, bx + x, by + y, w, h);
      } else if (cmd == 'F') {
        x = fs.buf[ptr++];
        y = fs.buf[ptr++];
        w = fs.buf[ptr++];
        h = fs.buf[ptr++];
        clr   = fs.buf[ptr++];
        clr <<= 8;
        clr  |= fs.buf[ptr++];
        tft.fillRect(bx + x, by + y, w, h, clr);
      } else if (cmd == 'W') {
        n = fs.buf[ptr++];
        do {
          if (callback && (cmd = callback())) {
            fs.close();
            return cmd;
          }
        } while ((millis() - time) < n);
        time = millis();
      } else if (cmd == 'w') {
        n   = fs.buf[ptr++];
        n <<= 8;
        n  |= fs.buf[ptr++];
        do {
          if (callback && (cmd = callback())) {
            fs.close();
            return cmd;
          }
        } while ((millis() - time) < n);
        time = millis();
      } else if (cmd == 'N') {
        break;
      } else if (cmd == 'S') {
        n = fs.buf[ptr++];
        fs.seek(n);
        break;
      } else if (cmd == 's') {
        n   = fs.buf[ptr++];
        n <<= 8;
        n  |= fs.buf[ptr++];
        fs.seek(n);
        break;
      } else if (cmd == 'A') {
        bx = (tft.width  - fs.buf[ptr++]) >> 1;
        by = (tft.height - fs.buf[ptr++]) >> 1;
      } else if (cmd == 'Z') {
        fs.close();
        return SMF_OK;
      }
    }
  }
  fs.close();
  return SMF_OK;
}

uint8_t tft_drawGCI(uint32_t sect, uint16_t offs, int16_t x, int16_t y, uint8_t (*callback)(void)) {
  uint32_t time = millis();
  uint16_t ptr = offs;
  int16_t w, h;
  uint16_t ms, frames, i;
  uint8_t ret;
  if (fs.seek(sect) || fs.read()) return SMF_ERROR;
  w = fs.buf[ptr++]; w <<= 8; w |= fs.buf[ptr++]; if (!w) return SMF_ERROR;
  h = fs.buf[ptr++]; h <<= 8; h |= fs.buf[ptr++]; if (!h) return SMF_ERROR;
  if (fs.buf[ptr++] != 0x10) return SMF_ERROR;
  if ((ms = fs.buf[ptr++])) {
    frames = fs.buf[ptr++]; frames <<= 8; frames |= fs.buf[ptr++];
    if (!frames) return SMF_ERROR;
  } else {
    frames = 0;
  }
  if (frames < 2) {
    tft_blit(ptr, x, y, w, h);
    return SMF_OK;
  } else {
    for (;;) {
      for (i = 0; i < frames; i++) {
        ptr = tft_blit(ptr, x, y, w, h);
        do { if (callback && (ret = callback())) return ret; }
        while ((millis() - time) < ms);
        time = millis();
      }
      if (fs.seek(sect) || fs.read()) return SMF_ERROR;
      ptr = offs + 8;
    }
  }
}
