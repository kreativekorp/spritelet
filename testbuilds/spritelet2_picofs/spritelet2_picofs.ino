#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include "clouds.h"
#include "picofs_diskio.h"
#include "picofs_pins.h"
#include "picofs.h"
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

void lena(void) {
	uint16_t ptr;
	if (!fs.open("LENA128.565")) {
		tft.setRotation(0);
		tft.setAddrWindow(0, 0, 127, 127);
		while (!fs.read()) {
			TFT_DC_PORT |=  TFT_DC_MASK;
			TFT_CS_PORT &=~ TFT_CS_MASK;
			for (ptr = 0; ptr < 512; ptr++) {
				SPI.transfer(fs.buf[ptr]);
			}
			TFT_CS_PORT |=  TFT_CS_MASK;
		}
		fs.close();
		while (!input_get());
		delay(200);
		while (input_get());
	}
}
