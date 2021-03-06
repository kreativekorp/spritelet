#ifndef __SPRITELET_FONT__
#define __SPRITELET_FONT__

#include <Arduino.h>
#include <avr/pgmspace.h>

void tft_drawChar(int16_t x, int16_t y, unsigned char ch, uint16_t bg, uint16_t fg);
void tft_drawString(int16_t x, int16_t y, char * str, uint16_t bg, uint16_t fg);
void tft_drawStringWrapped(int16_t x, int16_t y, int16_t w, char * str, uint16_t bg, uint16_t fg);

#define BUILTIN_FONT_WIDTH    6
#define BUILTIN_FONT_HEIGHT  12
#define BUILTIN_FONT_ASCENT   9
#define BUILTIN_FONT_DESCENT  3

const uint8_t PROGMEM builtin_font[256][9] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* null */
  { 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* grave */
  { 0x00, 0x02, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* dbl acute */
  { 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* dot */
  { 0x00, 0x85, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ring */
  { 0x00, 0x05, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* caron */
  { 0x00, 0x08, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* breve */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x0C }, /* ogonek */
  { 0x00, 0x00, 0x0F, 0x46, 0x54, 0x4F, 0x00, 0x00, 0x00 }, /* bksp */
  { 0x00, 0x00, 0x12, 0x2B, 0xE2, 0x92, 0x00, 0x00, 0x00 }, /* tab */
  { 0x00, 0x00, 0x36, 0x00, 0x85, 0x22, 0x00, 0x00, 0x00 }, /* enter */
  { 0x00, 0x00, 0x36, 0x40, 0x81, 0x06, 0x00, 0x00, 0x00 }, /* alt */
  { 0x00, 0x02, 0x14, 0x8B, 0x65, 0x14, 0x70, 0x00, 0x00 }, /* shift */
  { 0x00, 0x00, 0x0A, 0x4B, 0xE4, 0x08, 0x00, 0x00, 0x00 }, /* return */
  { 0x00, 0x03, 0x10, 0xF9, 0x24, 0x92, 0x48, 0x00, 0x00 }, /* fi */
  { 0x00, 0x03, 0x92, 0xE9, 0x24, 0x92, 0x48, 0x00, 0x00 }, /* fl */
  { 0x02, 0x8C, 0x30, 0xA0, 0x02, 0x8C, 0x30, 0xA0, 0x00 }, /* KK */
  { 0x00, 0x00, 0x42, 0x10, 0x84, 0x20, 0x00, 0x00, 0x00 }, /* fract */
  { 0x00, 0x30, 0x82, 0x10, 0x42, 0x28, 0x41, 0x00, 0x00 }, /* sqrt */
  { 0x03, 0xE8, 0x10, 0x20, 0x42, 0x10, 0x83, 0xE0, 0x00 }, /* sum */
  { 0x03, 0xE5, 0x14, 0x51, 0x45, 0x14, 0x51, 0x40, 0x00 }, /* prod */
  { 0x00, 0x62, 0x08, 0x20, 0x82, 0x08, 0x23, 0x00, 0x00 }, /* int */
  { 0x00, 0x02, 0x08, 0x51, 0x45, 0x22, 0xF8, 0x00, 0x00 }, /* delta */
  { 0x00, 0x07, 0x22, 0x8A, 0x28, 0x94, 0xD8, 0x00, 0x00 }, /* omega */
  { 0x00, 0x00, 0x00, 0xF9, 0x45, 0x14, 0x50, 0x00, 0x00 }, /* pi */
  { 0x00, 0x06, 0x04, 0x6A, 0x68, 0xA2, 0x70, 0x00, 0x00 }, /* deriv */
  { 0x00, 0x00, 0x00, 0x03, 0x6A, 0xB6, 0x00, 0x00, 0x00 }, /* inf */
  { 0x00, 0x00, 0x38, 0xCA, 0xA0, 0x9C, 0x00, 0x00, 0x00 }, /* esc */
  { 0x00, 0x01, 0x98, 0x81, 0x81, 0x80, 0xF8, 0x00, 0x00 }, /* <= */
  { 0x00, 0x00, 0x04, 0xF8, 0x8F, 0x90, 0x00, 0x00, 0x00 }, /* != */
  { 0x00, 0x0C, 0x0C, 0x08, 0xCC, 0x00, 0xF8, 0x00, 0x00 }, /* >= */
  { 0x00, 0x00, 0x1A, 0xB0, 0x06, 0xAC, 0x00, 0x00, 0x00 }, /* approx */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* space */
  { 0x00, 0x02, 0x08, 0x20, 0x82, 0x00, 0x20, 0x00, 0x00 }, /* ! */
  { 0x01, 0x45, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* " */
  { 0x00, 0x05, 0x3E, 0x53, 0xE5, 0x00, 0x00, 0x00, 0x00 }, /* # */
  { 0x00, 0x87, 0x2A, 0xA1, 0xC2, 0xAA, 0x70, 0x80, 0x00 }, /* $ */
  { 0x00, 0x04, 0x2A, 0x50, 0x85, 0x2A, 0x10, 0x00, 0x00 }, /* % */
  { 0x00, 0x04, 0x28, 0xA1, 0x0A, 0xA4, 0x68, 0x00, 0x00 }, /* & */
  { 0x00, 0x42, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ' */
  { 0x00, 0x42, 0x08, 0x41, 0x04, 0x08, 0x20, 0x40, 0x00 }, /* ( */
  { 0x01, 0x02, 0x08, 0x10, 0x41, 0x08, 0x21, 0x00, 0x00 }, /* ) */
  { 0x00, 0x02, 0x2A, 0x72, 0xA2, 0x00, 0x00, 0x00, 0x00 }, /* * */
  { 0x00, 0x00, 0x08, 0x23, 0xE2, 0x08, 0x00, 0x00, 0x00 }, /* + */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x84, 0x00 }, /* , */
  { 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00 }, /* - */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00 }, /* . */
  { 0x00, 0x20, 0x84, 0x10, 0x84, 0x10, 0x82, 0x00, 0x00 }, /* / */
  { 0x00, 0x07, 0x22, 0x9A, 0xAC, 0xA2, 0x70, 0x00, 0x00 }, /* 0 */
  { 0x00, 0x02, 0x38, 0x20, 0x82, 0x08, 0xF8, 0x00, 0x00 }, /* 1 */
  { 0x00, 0x07, 0x22, 0x08, 0x42, 0x10, 0xF8, 0x00, 0x00 }, /* 2 */
  { 0x00, 0x07, 0x22, 0x08, 0xC0, 0xA2, 0x70, 0x00, 0x00 }, /* 3 */
  { 0x00, 0x01, 0x0C, 0x52, 0x4F, 0x84, 0x10, 0x00, 0x00 }, /* 4 */
  { 0x00, 0x0F, 0xA0, 0xF0, 0x20, 0xA2, 0x70, 0x00, 0x00 }, /* 5 */
  { 0x00, 0x07, 0x20, 0xF2, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* 6 */
  { 0x00, 0x0F, 0x82, 0x10, 0x42, 0x08, 0x20, 0x00, 0x00 }, /* 7 */
  { 0x00, 0x07, 0x22, 0x89, 0xC8, 0xA2, 0x70, 0x00, 0x00 }, /* 8 */
  { 0x00, 0x07, 0x22, 0x8A, 0x27, 0x82, 0x70, 0x00, 0x00 }, /* 9 */
  { 0x00, 0x00, 0x00, 0x61, 0x80, 0x18, 0x60, 0x00, 0x00 }, /* : */
  { 0x00, 0x00, 0x00, 0x61, 0x80, 0x18, 0x60, 0x84, 0x00 }, /* ; */
  { 0x00, 0x00, 0x06, 0x62, 0x06, 0x06, 0x00, 0x00, 0x00 }, /* < */
  { 0x00, 0x00, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x00, 0x00 }, /* = */
  { 0x00, 0x00, 0x30, 0x30, 0x23, 0x30, 0x00, 0x00, 0x00 }, /* > */
  { 0x00, 0x07, 0x22, 0x08, 0x42, 0x00, 0x20, 0x00, 0x00 }, /* ? */
  { 0x00, 0x07, 0x22, 0xAB, 0xAB, 0xA0, 0x70, 0x00, 0x00 }, /* @ */
  { 0x00, 0x07, 0x22, 0x8B, 0xE8, 0xA2, 0x88, 0x00, 0x00 }, /* A */
  { 0x00, 0x0F, 0x22, 0x8B, 0xC8, 0xA2, 0xF0, 0x00, 0x00 }, /* B */
  { 0x00, 0x07, 0x22, 0x82, 0x08, 0x22, 0x70, 0x00, 0x00 }, /* C */
  { 0x00, 0x0F, 0x22, 0x8A, 0x28, 0xA2, 0xF0, 0x00, 0x00 }, /* D */
  { 0x00, 0x0F, 0xA0, 0x83, 0x88, 0x20, 0xF8, 0x00, 0x00 }, /* E */
  { 0x00, 0x0F, 0xA0, 0x83, 0x88, 0x20, 0x80, 0x00, 0x00 }, /* F */
  { 0x00, 0x07, 0x22, 0x82, 0x68, 0xA2, 0x70, 0x00, 0x00 }, /* G */
  { 0x00, 0x08, 0xA2, 0x8B, 0xE8, 0xA2, 0x88, 0x00, 0x00 }, /* H */
  { 0x00, 0x07, 0x08, 0x20, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* I */
  { 0x00, 0x03, 0x84, 0x10, 0x41, 0x24, 0x60, 0x00, 0x00 }, /* J */
  { 0x00, 0x08, 0xA4, 0xA3, 0x0A, 0x24, 0x88, 0x00, 0x00 }, /* K */
  { 0x00, 0x08, 0x20, 0x82, 0x08, 0x20, 0xF8, 0x00, 0x00 }, /* L */
  { 0x00, 0x08, 0xB6, 0xAA, 0xA8, 0xA2, 0x88, 0x00, 0x00 }, /* M */
  { 0x00, 0x08, 0xA2, 0xCA, 0xA9, 0xA2, 0x88, 0x00, 0x00 }, /* N */
  { 0x00, 0x07, 0x22, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* O */
  { 0x00, 0x0F, 0x22, 0x8B, 0xC8, 0x20, 0x80, 0x00, 0x00 }, /* P */
  { 0x00, 0x07, 0x22, 0x8A, 0x2A, 0xA4, 0x68, 0x00, 0x00 }, /* Q */
  { 0x00, 0x0F, 0x22, 0x8B, 0xCA, 0x24, 0x88, 0x00, 0x00 }, /* R */
  { 0x00, 0x07, 0x22, 0x81, 0xC0, 0xA2, 0x70, 0x00, 0x00 }, /* S */
  { 0x00, 0x0F, 0x88, 0x20, 0x82, 0x08, 0x20, 0x00, 0x00 }, /* T */
  { 0x00, 0x08, 0xA2, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* U */
  { 0x00, 0x08, 0xA2, 0x51, 0x45, 0x08, 0x20, 0x00, 0x00 }, /* V */
  { 0x00, 0x08, 0xA2, 0x8A, 0xAA, 0xAA, 0x50, 0x00, 0x00 }, /* W */
  { 0x00, 0x08, 0xA2, 0x50, 0x85, 0x22, 0x88, 0x00, 0x00 }, /* X */
  { 0x00, 0x08, 0xA2, 0x50, 0x82, 0x08, 0x20, 0x00, 0x00 }, /* Y */
  { 0x00, 0x0F, 0x82, 0x10, 0x84, 0x20, 0xF8, 0x00, 0x00 }, /* Z */
  { 0x01, 0xC4, 0x10, 0x41, 0x04, 0x10, 0x41, 0xC0, 0x00 }, /* [ */
  { 0x02, 0x08, 0x10, 0x40, 0x81, 0x04, 0x08, 0x20, 0x00 }, /* \ */
  { 0x01, 0xC1, 0x04, 0x10, 0x41, 0x04, 0x11, 0xC0, 0x00 }, /* ] */
  { 0x00, 0x02, 0x14, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ^ */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80 }, /* _ */
  { 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ` */
  { 0x00, 0x00, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a */
  { 0x00, 0x08, 0x20, 0xB3, 0x28, 0xA2, 0xF0, 0x00, 0x00 }, /* b */
  { 0x00, 0x00, 0x00, 0x72, 0x28, 0x20, 0x78, 0x00, 0x00 }, /* c */
  { 0x00, 0x00, 0x82, 0x7A, 0x28, 0xA6, 0x68, 0x00, 0x00 }, /* d */
  { 0x00, 0x00, 0x00, 0x72, 0x2F, 0xA0, 0x70, 0x00, 0x00 }, /* e */
  { 0x00, 0x01, 0x88, 0x70, 0x82, 0x08, 0x20, 0x00, 0x00 }, /* f */
  { 0x00, 0x00, 0x00, 0x7A, 0x28, 0xA6, 0x68, 0x27, 0x00 }, /* g */
  { 0x00, 0x08, 0x20, 0xB3, 0x28, 0xA2, 0x88, 0x00, 0x00 }, /* h */
  { 0x00, 0x02, 0x00, 0x60, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* i */
  { 0x00, 0x02, 0x00, 0x20, 0x82, 0x08, 0x20, 0x8C, 0x00 }, /* j */
  { 0x00, 0x08, 0x20, 0x92, 0x8E, 0x24, 0x88, 0x00, 0x00 }, /* k */
  { 0x00, 0x06, 0x08, 0x20, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* l */
  { 0x00, 0x00, 0x00, 0xD2, 0xAA, 0xAA, 0xA8, 0x00, 0x00 }, /* m */
  { 0x00, 0x00, 0x00, 0xB3, 0x28, 0xA2, 0x88, 0x00, 0x00 }, /* n */
  { 0x00, 0x00, 0x00, 0x72, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* o */
  { 0x00, 0x00, 0x00, 0xB3, 0x28, 0xA2, 0xF2, 0x08, 0x00 }, /* p */
  { 0x00, 0x00, 0x00, 0x7A, 0x28, 0xA6, 0x68, 0x20, 0x80 }, /* q */
  { 0x00, 0x00, 0x00, 0xB3, 0x28, 0x20, 0x80, 0x00, 0x00 }, /* r */
  { 0x00, 0x00, 0x00, 0x7A, 0x07, 0x02, 0xF0, 0x00, 0x00 }, /* s */
  { 0x00, 0x02, 0x08, 0x70, 0x82, 0x08, 0x18, 0x00, 0x00 }, /* t */
  { 0x00, 0x00, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x00, 0x00 }, /* u */
  { 0x00, 0x00, 0x00, 0x8A, 0x25, 0x14, 0x20, 0x00, 0x00 }, /* v */
  { 0x00, 0x00, 0x00, 0x8A, 0xAA, 0xAA, 0x50, 0x00, 0x00 }, /* w */
  { 0x00, 0x00, 0x00, 0x89, 0x42, 0x14, 0x88, 0x00, 0x00 }, /* x */
  { 0x00, 0x00, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x27, 0x00 }, /* y */
  { 0x00, 0x00, 0x00, 0xF8, 0x42, 0x10, 0xF8, 0x00, 0x00 }, /* z */
  { 0x00, 0x42, 0x08, 0x21, 0x02, 0x08, 0x20, 0x40, 0x00 }, /* { */
  { 0x00, 0x82, 0x08, 0x20, 0x82, 0x08, 0x20, 0x80, 0x00 }, /* | */
  { 0x01, 0x02, 0x08, 0x20, 0x42, 0x08, 0x21, 0x00, 0x00 }, /* } */
  { 0x00, 0x06, 0xAC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ~ */
  { 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE }, /* cursor */
  { 0x00, 0x03, 0x12, 0xE1, 0x0E, 0x12, 0x30, 0x00, 0x00 }, /* euro */
  { 0x00, 0x82, 0x14, 0x52, 0x25, 0x14, 0x20, 0x80, 0x00 }, /* lozenge */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x42, 0x00 }, /* , */
  { 0x00, 0x01, 0x88, 0x70, 0x82, 0x08, 0x20, 0x8C, 0x00 }, /* florin */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0xD9, 0x29, 0x00 }, /* ,, */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0xA8, 0x00, 0x00 }, /* ... */
  { 0x00, 0x02, 0x1C, 0x20, 0x82, 0x00, 0x00, 0x00, 0x00 }, /* dagger */
  { 0x00, 0x02, 0x1C, 0x21, 0xC2, 0x00, 0x00, 0x00, 0x00 }, /* dbl dagger */
  { 0x00, 0x02, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* circum */
  { 0x00, 0x04, 0x2A, 0x50, 0x86, 0xB5, 0x28, 0x00, 0x00 }, /* per mille */
  { 0x50, 0x80, 0x1C, 0x81, 0xC0, 0xA2, 0x70, 0x00, 0x00 }, /* S caron */
  { 0x00, 0x00, 0x04, 0x21, 0x02, 0x04, 0x00, 0x00, 0x00 }, /* < */
  { 0x00, 0x07, 0xA8, 0xA2, 0xCA, 0x28, 0x78, 0x00, 0x00 }, /* OE */
  { 0x00, 0x04, 0x10, 0x63, 0x04, 0x10, 0x78, 0x00, 0x00 }, /* L/ */
  { 0x50, 0x80, 0x3E, 0x10, 0x84, 0x20, 0xF8, 0x00, 0x00 }, /* Z caron */
  { 0x00, 0x00, 0x00, 0x60, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* dotless i */
  { 0x00, 0x00, 0x14, 0xA9, 0xCA, 0x94, 0x00, 0x00, 0x00 }, /* meta */
  { 0x10, 0x83, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ` */
  { 0x30, 0xC1, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ' */
  { 0x4A, 0x4D, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* `` */
  { 0xDB, 0x64, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* '' */
  { 0x00, 0x00, 0x1C, 0xFB, 0xEF, 0x9C, 0x00, 0x00, 0x00 }, /* bullet */
  { 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00 }, /* en dash */
  { 0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00 }, /* em dash */
  { 0x00, 0x06, 0xAC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* tilde */
  { 0x00, 0x0F, 0x57, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* (TM) */
  { 0x01, 0x42, 0x00, 0x7A, 0x07, 0x02, 0xF0, 0x00, 0x00 }, /* s caron */
  { 0x00, 0x00, 0x10, 0x20, 0x42, 0x10, 0x00, 0x00, 0x00 }, /* > */
  { 0x00, 0x00, 0x00, 0x52, 0xAB, 0xA8, 0x58, 0x00, 0x00 }, /* oe */
  { 0x00, 0x06, 0x08, 0x31, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* l/ */
  { 0x01, 0x42, 0x00, 0xF8, 0x42, 0x10, 0xF8, 0x00, 0x00 }, /* z caron */
  { 0x50, 0x08, 0xA2, 0x50, 0x82, 0x08, 0x20, 0x00, 0x00 }, /* Y" */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* nbsp */
  { 0x00, 0x00, 0x00, 0x20, 0x02, 0x08, 0x20, 0x82, 0x00 }, /* inv ! */
  { 0x00, 0x00, 0x08, 0x72, 0xAA, 0x28, 0x78, 0x80, 0x00 }, /* cent */
  { 0x00, 0x06, 0x24, 0x83, 0x88, 0x22, 0xF0, 0x00, 0x00 }, /* pound */
  { 0x00, 0x08, 0x9C, 0x51, 0xC8, 0x80, 0x00, 0x00, 0x00 }, /* curr */
  { 0x00, 0x08, 0xA2, 0x50, 0x8F, 0x88, 0x20, 0x00, 0x00 }, /* yen */
  { 0x00, 0x82, 0x08, 0x20, 0x02, 0x08, 0x20, 0x80, 0x00 }, /* broken bar */
  { 0x00, 0x03, 0x90, 0x72, 0x27, 0x04, 0xE0, 0x00, 0x00 }, /* section */
  { 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* dier */
  { 0x00, 0x0F, 0x02, 0x6A, 0x28, 0x9A, 0x0B, 0xC0, 0x00 }, /* (C) */
  { 0x01, 0x83, 0x14, 0x30, 0x07, 0x00, 0x00, 0x00, 0x00 }, /* ^a */
  { 0x00, 0x00, 0x0A, 0x52, 0x85, 0x0A, 0x00, 0x00, 0x00 }, /* << */
  { 0x00, 0x00, 0x00, 0x03, 0xE0, 0x82, 0x00, 0x00, 0x00 }, /* not */
  { 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00 }, /* shy */
  { 0x00, 0x0F, 0x02, 0xCA, 0xAC, 0xAA, 0x0B, 0xC0, 0x00 }, /* (R) */
  { 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* macron */
  { 0x01, 0x89, 0x24, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* degree */
  { 0x00, 0x02, 0x08, 0xF8, 0x82, 0x00, 0xF8, 0x00, 0x00 }, /* +/- */
  { 0x01, 0x81, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ^2 */
  { 0x01, 0x83, 0x04, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ^3 */
  { 0x00, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* acute */
  { 0x00, 0x00, 0x00, 0x8A, 0x28, 0xB6, 0xAA, 0x08, 0x00 }, /* mu */
  { 0x00, 0x07, 0xAA, 0xAA, 0xA7, 0x8A, 0x28, 0x00, 0x00 }, /* paragraph */
  { 0x00, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00 }, /* dot */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x18 }, /* cedilla */
  { 0x00, 0x86, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ^1 */
  { 0x00, 0x85, 0x14, 0x20, 0x07, 0x00, 0x00, 0x00, 0x00 }, /* ^o */
  { 0x00, 0x00, 0x28, 0x50, 0xA5, 0x28, 0x00, 0x00, 0x00 }, /* >> */
  { 0x01, 0x0C, 0x12, 0xF0, 0x84, 0xA6, 0x38, 0x20, 0x00 }, /* 1/4 */
  { 0x01, 0x0C, 0x12, 0xF0, 0x87, 0x22, 0x10, 0xE0, 0x00 }, /* 1/2 */
  { 0x03, 0x06, 0x0A, 0xD0, 0x84, 0xA6, 0x38, 0x20, 0x00 }, /* 3/4 */
  { 0x00, 0x00, 0x00, 0x20, 0x02, 0x10, 0x82, 0x27, 0x00 }, /* inv ? */
  { 0x40, 0x80, 0x1C, 0x8A, 0x2F, 0xA2, 0x88, 0x00, 0x00 }, /* A` */
  { 0x10, 0x80, 0x1C, 0x8A, 0x2F, 0xA2, 0x88, 0x00, 0x00 }, /* A' */
  { 0x21, 0x40, 0x1C, 0x8A, 0x2F, 0xA2, 0x88, 0x00, 0x00 }, /* A^ */
  { 0x6A, 0xC0, 0x1C, 0x8A, 0x2F, 0xA2, 0x88, 0x00, 0x00 }, /* A~ */
  { 0x50, 0x07, 0x22, 0x8B, 0xE8, 0xA2, 0x88, 0x00, 0x00 }, /* A" */
  { 0x21, 0x42, 0x1C, 0x8A, 0x2F, 0xA2, 0x88, 0x00, 0x00 }, /* A ring */
  { 0x00, 0x07, 0xA8, 0xA3, 0xCA, 0x28, 0xB8, 0x00, 0x00 }, /* AE */
  { 0x00, 0x07, 0x22, 0x82, 0x08, 0x22, 0x70, 0x84, 0x00 }, /* C cedilla */
  { 0x40, 0x80, 0x3E, 0x83, 0x88, 0x20, 0xF8, 0x00, 0x00 }, /* E` */
  { 0x10, 0x80, 0x3E, 0x83, 0x88, 0x20, 0xF8, 0x00, 0x00 }, /* E' */
  { 0x21, 0x40, 0x3E, 0x83, 0x88, 0x20, 0xF8, 0x00, 0x00 }, /* E^ */
  { 0x50, 0x0F, 0xA0, 0x83, 0x88, 0x20, 0xF8, 0x00, 0x00 }, /* E" */
  { 0x40, 0x80, 0x1C, 0x20, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* I` */
  { 0x10, 0x80, 0x1C, 0x20, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* I' */
  { 0x21, 0x40, 0x1C, 0x20, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* I^ */
  { 0x50, 0x07, 0x08, 0x20, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* I" */
  { 0x00, 0x07, 0x12, 0x4B, 0xA4, 0x92, 0x70, 0x00, 0x00 }, /* Eth */
  { 0x6A, 0xC0, 0x22, 0xCA, 0xA9, 0xA2, 0x88, 0x00, 0x00 }, /* N~ */
  { 0x40, 0x80, 0x1C, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* O` */
  { 0x10, 0x80, 0x1C, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* O' */
  { 0x21, 0x40, 0x1C, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* O^ */
  { 0x6A, 0xC0, 0x1C, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* O~ */
  { 0x50, 0x07, 0x22, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* O" */
  { 0x00, 0x00, 0x22, 0x50, 0x85, 0x22, 0x00, 0x00, 0x00 }, /* mult */
  { 0x00, 0x06, 0xA4, 0xAA, 0xAA, 0x92, 0xB0, 0x00, 0x00 }, /* O/ */
  { 0x40, 0x80, 0x22, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* U` */
  { 0x10, 0x80, 0x22, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* U' */
  { 0x21, 0x40, 0x22, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* U^ */
  { 0x50, 0x08, 0xA2, 0x8A, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* U" */
  { 0x10, 0x80, 0x22, 0x89, 0x42, 0x08, 0x20, 0x00, 0x00 }, /* Y' */
  { 0x00, 0x08, 0x3C, 0x8A, 0x28, 0xBC, 0x80, 0x00, 0x00 }, /* Thorn */
  { 0x00, 0x07, 0x22, 0x8A, 0xC8, 0xA2, 0xB0, 0x00, 0x00 }, /* sz */
  { 0x01, 0x02, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a` */
  { 0x00, 0x42, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a' */
  { 0x00, 0x85, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a^ */
  { 0x01, 0xAB, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a~ */
  { 0x00, 0x05, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a" */
  { 0x21, 0x42, 0x00, 0x70, 0x27, 0xA2, 0x78, 0x00, 0x00 }, /* a ring */
  { 0x00, 0x00, 0x00, 0x50, 0xA7, 0xA8, 0x78, 0x00, 0x00 }, /* ae */
  { 0x00, 0x00, 0x00, 0x72, 0x28, 0x20, 0x78, 0x84, 0x00 }, /* c cedilla */
  { 0x01, 0x02, 0x00, 0x72, 0x2F, 0xA0, 0x70, 0x00, 0x00 }, /* e` */
  { 0x00, 0x42, 0x00, 0x72, 0x2F, 0xA0, 0x70, 0x00, 0x00 }, /* e' */
  { 0x00, 0x85, 0x00, 0x72, 0x2F, 0xA0, 0x70, 0x00, 0x00 }, /* e^ */
  { 0x00, 0x05, 0x00, 0x72, 0x2F, 0xA0, 0x70, 0x00, 0x00 }, /* e" */
  { 0x01, 0x02, 0x00, 0x60, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* i` */
  { 0x00, 0x42, 0x00, 0x60, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* i' */
  { 0x00, 0x85, 0x00, 0x60, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* i^ */
  { 0x00, 0x05, 0x00, 0x60, 0x82, 0x08, 0x70, 0x00, 0x00 }, /* i" */
  { 0x00, 0x06, 0x84, 0x6A, 0x68, 0xA2, 0x70, 0x00, 0x00 }, /* eth */
  { 0x01, 0xAB, 0x00, 0xB3, 0x28, 0xA2, 0x88, 0x00, 0x00 }, /* n~ */
  { 0x01, 0x02, 0x00, 0x72, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* o` */
  { 0x00, 0x42, 0x00, 0x72, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* o' */
  { 0x00, 0x85, 0x00, 0x72, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* o^ */
  { 0x01, 0xAB, 0x00, 0x72, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* o~ */
  { 0x00, 0x05, 0x00, 0x72, 0x28, 0xA2, 0x70, 0x00, 0x00 }, /* o" */
  { 0x00, 0x00, 0x08, 0x03, 0xE0, 0x08, 0x00, 0x00, 0x00 }, /* div */
  { 0x00, 0x00, 0x00, 0x6A, 0x4A, 0x92, 0xB0, 0x00, 0x00 }, /* o/ */
  { 0x01, 0x02, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x00, 0x00 }, /* u` */
  { 0x00, 0x42, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x00, 0x00 }, /* u' */
  { 0x00, 0x85, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x00, 0x00 }, /* u^ */
  { 0x00, 0x05, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x00, 0x00 }, /* u" */
  { 0x00, 0x42, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x27, 0x00 }, /* y' */
  { 0x00, 0x08, 0x20, 0xB3, 0x28, 0xA2, 0xF2, 0x08, 0x00 }, /* thorn */
  { 0x00, 0x05, 0x00, 0x8A, 0x28, 0xA6, 0x68, 0x27, 0x00 }, /* y" */
};

const uint8_t PROGMEM word_wrap_data[64] = {
  0xC0, 0x00, 0xFF, 0xF0, /* null - fl */
  0x00, 0x00, 0x03, 0x00, /* KK - approx */
  0xD0, 0xA0, 0x92, 0x11, /* space - / */
  0x00, 0x00, 0x00, 0x01, /* 0 - ? */
  0x00, 0x00, 0x00, 0x00, /* @ - O */
  0x00, 0x00, 0x02, 0x90, /* P - _ */
  0x00, 0x00, 0x00, 0x00, /* ` - o */
  0x00, 0x00, 0x02, 0x53, /* p - cursor */
  0x88, 0x90, 0x20, 0x00, /* euro - dotless i */
  0x00, 0x05, 0x00, 0x00, /* meta - Y" */
  0x2A, 0xA0, 0x00, 0x10, /* nbsp - macron */
  0xA0, 0x80, 0x00, 0x02, /* degree - inv ? */
  0x00, 0x00, 0x00, 0x00, /* A` - I" */
  0x00, 0x00, 0x00, 0x00, /* Eth - sz */
  0x00, 0x00, 0x00, 0x00, /* a` - i" */
  0x00, 0x00, 0x00, 0x00, /* eth - y" */
};

#endif
