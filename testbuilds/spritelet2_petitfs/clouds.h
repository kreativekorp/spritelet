#ifndef __CLOUDS_H__
#define __CLOUDS_H__

#include <Arduino.h>
#include <avr/pgmspace.h>

void clouds_setup(void);
void clouds_loop(void);

#define K 0x00,0x00
#define W 0xFF,0xFF
#define B 0x6C,0x3F
#define G 0x04,0xBE

const uint8_t PROGMEM cloud_tile_a[] = {
	B,B,B,B,B,B,B,B,B,K,K,B,B,B,B,B,B,B,B,B,B,B,B,B,
	B,B,B,B,B,B,B,B,K,W,W,K,B,B,B,B,B,B,B,B,B,B,B,B,
	B,B,B,B,B,B,B,K,W,W,W,K,B,B,B,B,B,B,B,B,B,B,B,B,
	B,B,B,B,B,B,K,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,B,B,
	B,B,B,B,K,K,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,B,
	B,B,B,K,W,W,W,G,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,
	B,B,B,K,W,W,G,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,
	B,B,B,K,W,G,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,
	B,B,K,W,W,G,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,
	B,K,W,W,W,G,G,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,
	B,K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,K,K,K,B,B,B,
	K,W,W,W,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,B,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,B,
	B,K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,W,K,
	B,B,K,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K
};

const uint8_t PROGMEM cloud_tile_b[] = {
	B,K,W,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,
	B,K,W,W,G,G,W,G,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,K,B,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,
	K,W,W,W,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,
	B,K,W,W,W,G,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,
	B,K,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,K,K,B,B,B,B,B,
	B,B,K,W,W,G,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,
	B,K,W,W,W,G,G,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,
	B,K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,K,K,K,B,B,B,
	K,W,W,W,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,B,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,B,
	B,K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,W,K,
	B,B,K,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K
};

const uint8_t PROGMEM cloud_tile_c[] = {
	B,K,W,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,
	B,K,W,W,G,G,W,G,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,G,W,W,W,W,K,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,K,B,B,
	K,W,W,G,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,
	K,W,W,W,G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,
	B,K,W,W,W,G,W,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,
	B,K,W,W,W,W,W,W,W,W,W,W,W,W,W,W,K,K,K,B,B,B,B,B,
	B,B,K,W,W,W,W,W,W,W,W,W,W,W,K,K,B,B,B,B,B,B,B,B,
	B,K,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,B,B,
	B,K,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,B,
	B,B,K,W,W,W,W,W,W,W,W,W,W,W,W,K,B,B,B,B,B,B,B,B,
	B,B,K,W,W,W,W,W,W,W,W,W,K,K,K,B,B,B,B,B,B,B,B,B,
	B,B,B,K,W,W,W,K,W,W,W,K,B,B,B,B,B,B,B,B,B,B,B,B,
	B,B,B,K,W,W,K,B,K,W,W,W,K,B,B,B,B,B,B,B,B,B,B,B,
	B,B,B,B,B,K,B,B,B,K,K,K,B,B,B,B,B,B,B,B,B,B,B,B,
	B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B
};

#endif
