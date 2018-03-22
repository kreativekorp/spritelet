#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include "clouds.h"
#include "st7735_commands.h"
#include "st7735_config.h"
#include "st7735_init.h"
#include "st7735_pins.h"
#include "st7735.h"

ST7735 tft = ST7735();

void setup(void) {
	randomSeed(analogRead(2));
	power_adc_disable();
	power_twi_disable();
	power_usart0_disable();
	power_timer1_disable();
	power_timer2_disable();

	tft.init(true);
	clouds_setup();
	tft.setBacklight(1);
}

void loop() {
	uint32_t t = millis();
	clouds_loop();
	while (millis() - t < 30);
}
