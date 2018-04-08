#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include "spi.h"
#include "st7735_commands.h"
#include "st7735_config.h"
#include "st7735_init.h"
#include "st7735_pins.h"
#include "st7735.h"
#include "toasters.h"

ST7735 tft = ST7735();

void setup(void) {
	randomSeed(analogRead(2));
	power_adc_disable();
	power_twi_disable();
	power_usart0_disable();
	power_timer1_disable();
	power_timer2_disable();

	tft.init(true);
	toasters_setup();
	tft.setBacklight(1);
}

void loop() {
	uint32_t t = millis();
	toasters_loop();
	while (millis() - t < 30);
}
