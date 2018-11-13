#include <Arduino.h>
#include "spritelet_font.h"
#include "spritelet_input.h"
#include "spritelet_util.h"
#include "st7735.h"

extern ST7735 tft;

void error_message(int16_t x1, char * m1, int16_t x2, char * m2, uint16_t clr) {
	tft.fillScreen(0);
	if (m2) {
		tft_drawString(x1, 52, m1, 0, clr);
		tft_drawString(x2, 64, m2, 0, clr);
	} else {
		tft_drawString(x1, 58, m1, 0, clr);
	}
	while (!input_get());
	delay(50);
	while (input_get());
	delay(50);
	tft.fillScreen(0);
}
