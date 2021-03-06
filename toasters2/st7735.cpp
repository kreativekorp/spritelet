/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "spi.h"
#include "st7735_commands.h"
#include "st7735_config.h"
#include "st7735_init.h"
#include "st7735_pins.h"
#include "st7735.h"

uint16_t ST7735::Color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void ST7735::init(boolean startSPI) {
	TFT_CS_DDR   |= TFT_CS_MASK  ; TFT_CS_PORT   |=  TFT_CS_MASK  ;
	TFT_DC_DDR   |= TFT_DC_MASK  ; TFT_DC_PORT   &=~ TFT_DC_MASK  ;
	TFT_RST_DDR  |= TFT_RST_MASK ; TFT_RST_PORT  &=~ TFT_RST_MASK ;
	TFT_LITE_DDR |= TFT_LITE_MASK; TFT_LITE_PORT &=~ TFT_LITE_MASK;

	if (startSPI) {
		SPI.begin();
		SPI.setClockDivider(SPI_CLOCK_DIV2);
		SPI.setDataMode(SPI_MODE0);
	}

	TFT_CS_PORT  &=~ TFT_CS_MASK ;
	TFT_RST_PORT |=  TFT_RST_MASK; delay(500);
	TFT_RST_PORT &=~ TFT_RST_MASK; delay(500);
	TFT_RST_PORT |=  TFT_RST_MASK; delay(500);
	TFT_CS_PORT  |=  TFT_CS_MASK ;

	commandList(st7735_init_commands);
	setRotation(0);
}

void ST7735::invertDisplay(boolean i) {
	writeCommand(i ? ST7735_INVON : ST7735_INVOFF);
}

void ST7735::setRotation(uint8_t r) {
	writeCommand(ST7735_MADCTL);
	switch (r & 3) {
		case 0:
			writeData(ST7735_R0_MADCTL);
			xstart = ST7735_R0_XSTART;
			ystart = ST7735_R0_YSTART;
			width  = ST7735_TFTWIDTH ;
			height = ST7735_TFTHEIGHT;
			break;
		case 1:
			writeData(ST7735_R1_MADCTL);
			xstart = ST7735_R1_XSTART;
			ystart = ST7735_R1_YSTART;
			width  = ST7735_TFTHEIGHT;
			height = ST7735_TFTWIDTH ;
			break;
		case 2:
			writeData(ST7735_R2_MADCTL);
			xstart = ST7735_R2_XSTART;
			ystart = ST7735_R2_YSTART;
			width  = ST7735_TFTWIDTH ;
			height = ST7735_TFTHEIGHT;
			break;
		case 3:
			writeData(ST7735_R3_MADCTL);
			xstart = ST7735_R3_XSTART;
			ystart = ST7735_R3_YSTART;
			width  = ST7735_TFTHEIGHT;
			height = ST7735_TFTWIDTH ;
			break;
	}
}

void ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	writeCommand(ST7735_CASET);
	writeData(0);
	writeData(x0 + xstart);
	writeData(0);
	writeData(x1 + xstart);
	writeCommand(ST7735_RASET);
	writeData(0);
	writeData(y0 + ystart);
	writeData(0);
	writeData(y1 + ystart);
	writeCommand(ST7735_RAMWR);
}

void ST7735::pushColor(uint16_t color) {
	TFT_DC_PORT |=  TFT_DC_MASK;
	TFT_CS_PORT &=~ TFT_CS_MASK;
	SPI.transfer(color >> 8);
	SPI.transfer(color);
	TFT_CS_PORT |=  TFT_CS_MASK;
}

void ST7735::pushColors(uint16_t * colors, uint16_t off, uint16_t len) {
	TFT_DC_PORT |=  TFT_DC_MASK;
	TFT_CS_PORT &=~ TFT_CS_MASK;
	colors += off;
	while (len--) {
		SPI.transfer((*colors) >> 8);
		SPI.transfer(*colors);
		colors++;
	}
	TFT_CS_PORT |=  TFT_CS_MASK;
}

void ST7735::fillScreen(uint16_t color) {
	uint16_t n = width; n *= height;
	uint8_t hi = (color >> 8), lo = color;
	setAddrWindow(0, 0, width - 1, height - 1);
	TFT_DC_PORT |=  TFT_DC_MASK;
	TFT_CS_PORT &=~ TFT_CS_MASK;
	while (n--) {
		SPI.transfer(hi);
		SPI.transfer(lo);
	}
	TFT_CS_PORT |=  TFT_CS_MASK;
}

void ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (x >= 0 && y >= 0 && x < width && y < height) {
		setAddrWindow(x, y, x, y);
		TFT_DC_PORT |=  TFT_DC_MASK;
		TFT_CS_PORT &=~ TFT_CS_MASK;
		SPI.transfer(color >> 8);
		SPI.transfer(color);
		TFT_CS_PORT |=  TFT_CS_MASK;
	}
}

void ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	if (x >= 0 && x < width) {
		if (y < 0) { h += y; y = 0; }
		if ((y + h - 1) >= height) h = height - y;
		if (h > 0) {
			uint8_t hi = (color >> 8), lo = color;
			setAddrWindow(x, y, x, y + h - 1);
			TFT_DC_PORT |=  TFT_DC_MASK;
			TFT_CS_PORT &=~ TFT_CS_MASK;
			while (h--) {
				SPI.transfer(hi);
				SPI.transfer(lo);
			}
			TFT_CS_PORT |=  TFT_CS_MASK;
		}
	}
}

void ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	if (y >= 0 && y < height) {
		if (x < 0) { w += x; x = 0; }
		if ((x + w - 1) >= width) w = width - x;
		if (w > 0) {
			uint8_t hi = (color >> 8), lo = color;
			setAddrWindow(x, y, x + w - 1, y);
			TFT_DC_PORT |=  TFT_DC_MASK;
			TFT_CS_PORT &=~ TFT_CS_MASK;
			while (w--) {
				SPI.transfer(hi);
				SPI.transfer(lo);
			}
			TFT_CS_PORT |=  TFT_CS_MASK;
		}
	}
}

void ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	if (x < 0) { w += x; x = 0; }
	if (y < 0) { h += y; y = 0; }
	if ((x + w - 1) >= width) w = width - x;
	if ((y + h - 1) >= height) h = height - y;
	if (w > 0 && h > 0) {
		uint16_t n = w; n *= h;
		uint8_t hi = (color >> 8), lo = color;
		setAddrWindow(x, y, x + w - 1, y + h - 1);
		TFT_DC_PORT |=  TFT_DC_MASK;
		TFT_CS_PORT &=~ TFT_CS_MASK;
		while (n--) {
			SPI.transfer(hi);
			SPI.transfer(lo);
		}
		TFT_CS_PORT |=  TFT_CS_MASK;
	}
}

void ST7735::setBacklight(boolean lite) {
	if (lite) TFT_LITE_PORT |=  TFT_LITE_MASK;
	else      TFT_LITE_PORT &=~ TFT_LITE_MASK;
}

void ST7735::writeCommand(uint8_t c) {
	TFT_DC_PORT &=~ TFT_DC_MASK;
	TFT_CS_PORT &=~ TFT_CS_MASK;
	SPI.transfer(c);
	TFT_CS_PORT |=  TFT_CS_MASK;
}

void ST7735::writeData(uint8_t d) {
	TFT_DC_PORT |=  TFT_DC_MASK;
	TFT_CS_PORT &=~ TFT_CS_MASK;
	SPI.transfer(d);
	TFT_CS_PORT |=  TFT_CS_MASK;
}

void ST7735::commandList(const uint8_t * addr) {
	uint8_t command, numArgs;
	while (1) {
		command = pgm_read_byte(addr++);
		if (command == ST7735_EOF) {
			return;
		} else if (command == ST7735_DELAY) {
			delay(pgm_read_byte(addr++));
		} else {
			writeCommand(command);
			numArgs = pgm_read_byte(addr++);
			while (numArgs--) writeData(pgm_read_byte(addr++));
		}
	}
}
