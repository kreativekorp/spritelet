#include <Arduino.h>
#include "picofs.h"
#include "spi.h"
#include "spritelet_bitmap.h"
#include "spritelet_input.h"
#include "st7735_pins.h"
#include "st7735.h"

extern ST7735 tft;
extern FATFS fs;

uint8_t tft_drawSMF(char * path) {
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
				n   = w;
				n  *= h;
				n <<= 1;
				if (n) {
					tft.setAddrWindow(bx + x, by + y, bx + x + w - 1, by + y + h - 1);
					clr = 512 - ptr;
					if (clr > n) clr = n;
					TFT_DC_PORT |=  TFT_DC_MASK;
					TFT_CS_PORT &=~ TFT_CS_MASK;
					SPI.writeBlock(fs.buf + ptr, clr);
					TFT_CS_PORT |=  TFT_CS_MASK;
					n -= clr;
					ptr += clr;
					while (n && !fs.read()) {
						clr = 512;
						if (clr > n) clr = n;
						TFT_DC_PORT |=  TFT_DC_MASK;
						TFT_CS_PORT &=~ TFT_CS_MASK;
						SPI.writeBlock(fs.buf, clr);
						TFT_CS_PORT |=  TFT_CS_MASK;
						n -= clr;
						ptr = clr;
					}
				}
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
					cmd = input_get();
					if (cmd &~ INPUT_VS) {
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
					cmd = input_get();
					if (cmd &~ INPUT_VS) {
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
