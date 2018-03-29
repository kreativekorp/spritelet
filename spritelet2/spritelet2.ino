#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include "clouds.h"
#include "picofs_diskio.h"
#include "picofs_pins.h"
#include "picofs.h"
#include "spi.h"
#include "spritelet_bitmap.h"
#include "spritelet_font.h"
#include "spritelet_input.h"
#include "spritelet_pins.h"
#include "st7735_commands.h"
#include "st7735_config.h"
#include "st7735_init.h"
#include "st7735_pins.h"
#include "st7735.h"

ST7735 tft = ST7735();
FATFS fs = FATFS();

void setup(void) {
	randomSeed(analogRead(2));
	power_adc_disable();
	power_twi_disable();
	power_usart0_disable();
	power_timer1_disable();
	power_timer2_disable();

	input_init();
	tft.init(true);
	tft.fillScreen(0);
	tft.setBacklight(1);

	fs.init(false);
	if (fs.mount()) {
		tft_drawString(25, 52, "Please insert", 0, -1);
		tft_drawString(25, 64, "microSD card.", 0, -1);
		do { delay(500); } while (fs.mount());
		tft.fillScreen(0);
	}

	clouds_setup();
}

void loop(void) {
	uint32_t t = millis();
	clouds_loop();
	do {
		switch (input_get()) {
			case INPUT_UP: delay(200); while (input_get()); tft.setRotation(3); tft.fillScreen(0x6C3F); break;
			case INPUT_DN: delay(200); while (input_get()); tft.setRotation(1); tft.fillScreen(0x6C3F); break;
			case INPUT_LT: delay(200); while (input_get()); tft.setRotation(2); tft.fillScreen(0x6C3F); break;
			case INPUT_RT: delay(200); while (input_get()); tft.setRotation(0); tft.fillScreen(0x6C3F); break;
			case INPUT_CTR: delay(200); while (input_get()); lena(); clouds_setup(); break;
		}
	} while (millis() - t < 30);
}

uint8_t lena_input(void) {
	return input_get() & ~INPUT_VS;
}

void lena(void) {
	uint8_t res;
	tft.setRotation(0);
	res = tft_drawSMF("NETSCAPE.SMF", lena_input);
	while (!res) res = lena_input();
	delay(50);
	while (input_get());
	delay(50);
}
