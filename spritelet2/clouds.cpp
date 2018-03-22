#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include "clouds.h"
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
	if ((y < 128) && (y > -h)) {
		if (y < 0) {
			ptr -= y * 48;
			h += y;
		} else if (r > 128) {
			h = 128 - y;
		}
		uint16_t n = h * 24;
		while (n--) {
			SPI.transfer(pgm_read_byte(ptr++));
			SPI.transfer(pgm_read_byte(ptr++));
		}
	}
	return r;
}

void clouds_setup(void) {
	tft.setRotation(3);
	tft.fillScreen(0x6C3F);
	for (uint8_t i = 0; i < N_CLOUDS; i++) cloud[i].column = 255;
	for (uint8_t i = 0; i < N_CLOUDS; i++) randomize(i, false);
}

void clouds_loop(void) {
	int16_t y;
	uint8_t i, x, r;
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
}
