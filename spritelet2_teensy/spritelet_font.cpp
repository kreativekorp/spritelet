#include <Arduino.h>
#include <avr/pgmspace.h>
#include "spi.h"
#include "spritelet_font.h"
#include "st7735_pins.h"
#include "st7735.h"

extern ST7735 tft;

void tft_drawChar(int16_t x, int16_t y, unsigned char ch, uint16_t bg, uint16_t fg) {
  if (x > (-BUILTIN_FONT_WIDTH) && x < tft.width && y > (-BUILTIN_FONT_HEIGHT) && y < tft.height) {
    uint8_t bh = (bg >> 8), bl = bg, fh = (fg >> 8), fl = fg;
    uint8_t cr, cc, cb, cp = 0, cm = 0;
    if (bg != fg) {
      tft.setAddrWindow(
        ((x > 0) ? x : 0),
        ((y > 0) ? y : 0),
        (((x + BUILTIN_FONT_WIDTH) < tft.width) ? (x + BUILTIN_FONT_WIDTH - 1) : (tft.width - 1)),
        (((y + BUILTIN_FONT_HEIGHT) < tft.height) ? (y + BUILTIN_FONT_HEIGHT - 1) : (tft.height - 1))
      );
      digitalWrite(TFT_DC, HIGH);
      digitalWrite(TFT_CS, LOW );
    }
    for (cr = 0; cr < BUILTIN_FONT_HEIGHT; cr++) {
      for (cc = 0; cc < BUILTIN_FONT_WIDTH; cc++) {
        if (!cm) {
          cb = pgm_read_byte(&builtin_font[ch][cp++]);
          cm = 0x80;
        }
        if (x >= 0 && x < tft.width && y >= 0 && y < tft.height) {
          if (cb & cm) {
            if (bg == fg) {
              tft.drawPixel(x, y, fg);
            } else {
              SPI_write(fh);
              SPI_write(fl);
            }
          } else {
            if (bg != fg) {
              SPI_write(bh);
              SPI_write(bl);
            }
          }
        }
        cm >>= 1;
        x++;
      }
      x -= BUILTIN_FONT_WIDTH;
      y++;
    }
    if (bg != fg) {
      digitalWrite(TFT_CS, HIGH);
    }
  }
}

void tft_drawString(int16_t x, int16_t y, char * str, uint16_t bg, uint16_t fg) {
  while (*str) {
    tft_drawChar(x, y, *str, bg, fg);
    x += BUILTIN_FONT_WIDTH;
    str++;
  }
}

static inline uint8_t wordWrapClass(unsigned char ch) {
  uint8_t wwd = pgm_read_byte(&word_wrap_data[ch >> 2]);
  wwd <<= ((ch & 0x03) << 1);
  return (wwd >> 6);
}

void tft_drawStringWrapped(int16_t x, int16_t y, int16_t w, char * str, uint16_t bg, uint16_t fg) {
  int16_t sx = x;      // start x
  int16_t mx = x + w;  // max x
  uint8_t wwc;         // word wrap class
  uint16_t ww;         // word width
  char * ws;           // word start
  while (*str) {
    if ((*str) == '\n' || (*str) == '\r') {
      x = sx;
      y += BUILTIN_FONT_HEIGHT;
      str++;
    } else {
      wwc = wordWrapClass(*str);
      switch (wwc) {
        case 1:  // wrap after
          if (x + BUILTIN_FONT_WIDTH > mx) {
            x = sx;
            y += BUILTIN_FONT_HEIGHT;
          }
          tft_drawChar(x, y, *str, bg, fg);
          x += BUILTIN_FONT_WIDTH;
          str++;
          break;
        case 3:  // whitespace
          if ((bg != fg) && (x + BUILTIN_FONT_WIDTH <= mx)) {
            tft.fillRect(x, y, BUILTIN_FONT_WIDTH, BUILTIN_FONT_HEIGHT, bg);
          }
          x += BUILTIN_FONT_WIDTH;
          str++;
          break;
        default:
          ww = 0;
          ws = str;
          while (wwc == 2) {
            // wrap before
            ww += BUILTIN_FONT_WIDTH;
            str++;
            wwc = wordWrapClass(*str);
          }
          while (wwc == 0) {
            // don't wrap
            ww += BUILTIN_FONT_WIDTH;
            str++;
            wwc = wordWrapClass(*str);
          }
          while (wwc == 1) {
            // wrap after
            ww += BUILTIN_FONT_WIDTH;
            str++;
            wwc = wordWrapClass(*str);
          }
          if (x + ww > mx) {
            x = sx;
            y += BUILTIN_FONT_HEIGHT;
          }
          while (ws != str) {
            if (x + BUILTIN_FONT_WIDTH > mx) {
              x = sx;
              y += BUILTIN_FONT_HEIGHT;
            }
            tft_drawChar(x, y, *ws, bg, fg);
            x += BUILTIN_FONT_WIDTH;
            ws++;
          }
          break;
      }
    }
  }
}
