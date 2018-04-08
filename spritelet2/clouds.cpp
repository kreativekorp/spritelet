#include <Arduino.h>
#include <avr/pgmspace.h>
#include "clouds.h"
#include "spi.h"
#include "spritelet_input.h"
#include "st7735_pins.h"
#include "st7735.h"

extern ST7735 tft;

#define N_CLOUDS 6

static struct {
	uint8_t column;
	uint8_t reps;
	int16_t y;
	int16_t endy;
	int16_t prev;
} cloud[N_CLOUDS];

static const uint8_t PROGMEM cloud_palette[] = {
	0x6C, 0x3F, 0x00, 0x00, 0x04, 0xBE, 0xFF, 0xFF,
};

static const uint8_t PROGMEM cloud_tile_a[] = {
	0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00,
	0x00, 0x01, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xD0, 0x00, 0x00,
	0x00, 0x5F, 0xFF, 0xF4, 0x00, 0x00, 0x01, 0xFE, 0xFF, 0xFD, 0x00, 0x00,
	0x01, 0xFB, 0xFF, 0xFD, 0x00, 0x00, 0x01, 0xEF, 0xFF, 0xFD, 0x00, 0x00,
	0x07, 0xEF, 0xFF, 0xFF, 0x40, 0x00, 0x1F, 0xEB, 0xFF, 0xFF, 0xD0, 0x00,
	0x1F, 0xAF, 0xFF, 0xFF, 0xF5, 0x40, 0x7F, 0xBF, 0xFF, 0xFF, 0xFF, 0xD0,
	0x7E, 0xBF, 0xFF, 0xFF, 0xBF, 0xD0, 0x7E, 0xBF, 0xFF, 0xFF, 0xEF, 0xF4,
	0x1F, 0xAF, 0xFF, 0xFF, 0xEF, 0xFD, 0x07, 0xAF, 0xFF, 0xFF, 0xFF, 0xFD,
};

static const uint8_t PROGMEM cloud_tile_b[] = {
	0x1F, 0xEB, 0xFF, 0xFF, 0xFF, 0xFD, 0x1F, 0xAE, 0xFF, 0xFF, 0xFB, 0xFD,
	0x7E, 0xBF, 0xFF, 0xFF, 0xEF, 0xF4, 0x7E, 0xBF, 0xFF, 0xFF, 0xFF, 0x50,
	0x7E, 0xBF, 0xFF, 0xFF, 0xFD, 0x00, 0x7F, 0xBF, 0xFF, 0xFF, 0xFF, 0x40,
	0x1F, 0xEF, 0xFF, 0xFF, 0xFD, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0x54, 0x00,
	0x07, 0xEF, 0xFF, 0xFF, 0x40, 0x00, 0x1F, 0xEB, 0xFF, 0xFF, 0xD0, 0x00,
	0x1F, 0xAF, 0xFF, 0xFF, 0xF5, 0x40, 0x7F, 0xBF, 0xFF, 0xFF, 0xFF, 0xD0,
	0x7E, 0xBF, 0xFF, 0xFF, 0xBF, 0xD0, 0x7E, 0xBF, 0xFF, 0xFF, 0xEF, 0xF4,
	0x1F, 0xAF, 0xFF, 0xFF, 0xEF, 0xFD, 0x07, 0xAF, 0xFF, 0xFF, 0xFF, 0xFD,
};

static const uint8_t PROGMEM cloud_tile_c[] = {
	0x1F, 0xEB, 0xFF, 0xFF, 0xFF, 0xFD, 0x1F, 0xAE, 0xFF, 0xFF, 0xFB, 0xFD,
	0x7E, 0xBF, 0xFF, 0xFF, 0xEF, 0xF4, 0x7E, 0xBF, 0xFF, 0xFF, 0xFF, 0x50,
	0x7E, 0xBF, 0xFF, 0xFF, 0xFD, 0x00, 0x7F, 0xBF, 0xFF, 0xFF, 0xFF, 0x40,
	0x1F, 0xEF, 0xFF, 0xFF, 0xFD, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0x54, 0x00,
	0x07, 0xFF, 0xFF, 0xF5, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xD0, 0x00, 0x00,
	0x1F, 0xFF, 0xFF, 0xF4, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFD, 0x00, 0x00,
	0x07, 0xFF, 0xFF, 0x54, 0x00, 0x00, 0x01, 0xFD, 0xFD, 0x00, 0x00, 0x00,
	0x01, 0xF4, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x10, 0x15, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static void randomize(uint8_t i, boolean offRight) {
	uint8_t j, tries = 0;
	int iy1, iy2, jy1, jy2, maxy = 2048;
	cloud[i].reps = random(4);
	do {
		cloud[i].column = random(4);
		if (offRight) cloud[i].y = (random(64) + 128) << 4;
		else cloud[i].y = random((cloud[i].reps + 2) * -16, 192) << 4;
		iy1 = cloud[i].y;
		iy2 = iy1 + (cloud[i].reps + 2) * 256 + 15;
		for (j = 0; j < N_CLOUDS; j++) {
			if ((i == j) || (cloud[i].column != cloud[j].column)) continue;
			jy1 = cloud[j].y;
			jy2 = jy1 + (cloud[j].reps + 2) * 256 + 15;
			if (jy2 > maxy) maxy = jy2;
			if ((jy1 <= iy2) && (jy2 >= iy1)) break;
		}
	} while ((j < N_CLOUDS) && (++tries < 5));
	if (tries > 4) cloud[i].y = maxy + 16;
	cloud[i].endy = (cloud[i].reps + 2) * -256 - 31;
	cloud[i].prev = -30000;
}

static int16_t blit(const uint8_t * ptr, int16_t y, uint8_t h) {
	int16_t r = y + h;
	uint8_t n, m, i;
	if ((y < 128) && (y > -h)) {
		if (y < 0) {
			ptr -= y * 6;
			h += y;
		} else if (r > 128) {
			h = 128 - y;
		}
		n = h * 6;
		while (n--) {
			m = pgm_read_byte(ptr++);
			i = (m >> 5) & 0x06; SPI.write(pgm_read_byte(&cloud_palette[i])); i++; SPI.write(pgm_read_byte(&cloud_palette[i]));
			i = (m >> 3) & 0x06; SPI.write(pgm_read_byte(&cloud_palette[i])); i++; SPI.write(pgm_read_byte(&cloud_palette[i]));
			i = (m >> 1) & 0x06; SPI.write(pgm_read_byte(&cloud_palette[i])); i++; SPI.write(pgm_read_byte(&cloud_palette[i]));
			i = (m << 1) & 0x06; SPI.write(pgm_read_byte(&cloud_palette[i])); i++; SPI.write(pgm_read_byte(&cloud_palette[i]));
		}
	}
	return r;
}

uint8_t clouds_setup(void) {
	tft.setRotation(3);
	tft.fillScreen(0x6C3F);
	for (uint8_t i = 0; i < N_CLOUDS; i++) cloud[i].column = 255;
	for (uint8_t i = 0; i < N_CLOUDS; i++) randomize(i, false);
	return 1;
}

uint8_t clouds_loop(void) {
	uint32_t t = millis();
	int16_t y; uint8_t i, x, r;
	for (i = 0; i < N_CLOUDS; i++) {
		y = cloud[i].y >> 4;
		if (y != cloud[i].prev) {
			cloud[i].prev = y;
			if (y < 128) {
				x = (cloud[i].column << 5) | 4;
				tft.setAddrWindow(x, ((y > 0) ? y : 0), x + 23, 127);
				TFT_DC_PORT |=  TFT_DC_MASK;
				TFT_CS_PORT &=~ TFT_CS_MASK;
				y = blit(cloud_tile_a, y, 16);
				for (r = 0; r < cloud[i].reps; r++) {
					y = blit(cloud_tile_b, y, 16);
				}
				blit(cloud_tile_c, y, 17);
				TFT_CS_PORT |=  TFT_CS_MASK;
			}
		}
		cloud[i].y -= ((cloud[i].column * 3) + 7);
		if (cloud[i].y < cloud[i].endy) randomize(i, true);
	}
	do { 
		if (input_get() == INPUT_CTR) {
			tft.setRotation(0);
			tft.fillScreen(0);
			delay(50);
			while (input_get());
			delay(50);
			return 0;
		}
	} while (millis() - t < 30);
	return 1;
}
