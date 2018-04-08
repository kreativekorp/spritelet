/**************************************************************************/
/* ST7735 Driver Library for Adafruit 1.44" TFT Breakout with SD Card.    */
/* Based on Adafruit_ST7735_Library but stripped down and with bug fixes. */
/* Written by Rebecca Bettencourt. MIT license.                           */
/**************************************************************************/

#ifndef __ST7735_H__
#define __ST7735_H__

#include <Arduino.h>

class ST7735 {
	public:
		uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

		void init(boolean startSPI);
		void invertDisplay(boolean i);
		void setRotation(uint8_t r);
		void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
		void pushColor(uint16_t color);
		void pushColors(uint16_t * colors, uint16_t off, uint16_t len);
		void fillScreen(uint16_t color);
		void drawPixel(int16_t x, int16_t y, uint16_t color);
		void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

		void setBacklight(boolean lite);

	private:
		uint8_t xstart, ystart, width, height;

		void writeCommand(uint8_t c);
		void writeData(uint8_t d);
		void commandList(const uint8_t * addr);
};

#endif
