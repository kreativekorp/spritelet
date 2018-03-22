#include <Arduino.h>
#include "spritelet_input.h"
#include "spritelet_pins.h"

void input_init(void) {
	VS_GND_DDR  |=  VS_GND_MASK ; VS_GND_PORT  &=~ VS_GND_MASK ;
	VS_POS_DDR  &=~ VS_POS_MASK ; VS_POS_PORT  |=  VS_POS_MASK ;
	JOY_COM_DDR |=  JOY_COM_MASK; JOY_COM_PORT &=~ JOY_COM_MASK;
	JOY_CTR_DDR &=~ JOY_CTR_MASK; JOY_CTR_PORT |=  JOY_CTR_MASK;
	JOY_UP_DDR  &=~ JOY_UP_MASK ; JOY_UP_PORT  |=  JOY_UP_MASK ;
	JOY_DN_DDR  &=~ JOY_DN_MASK ; JOY_DN_PORT  |=  JOY_DN_MASK ;
	JOY_LT_DDR  &=~ JOY_LT_MASK ; JOY_LT_PORT  |=  JOY_LT_MASK ;
	JOY_RT_DDR  &=~ JOY_RT_MASK ; JOY_RT_PORT  |=  JOY_RT_MASK ;
}

uint8_t input_get(void) {
	if (!(VS_POS_PIN  & VS_POS_MASK )) return INPUT_VS ;
	if (!(JOY_CTR_PIN & JOY_CTR_MASK)) return INPUT_CTR;
	if (!(JOY_UP_PIN  & JOY_UP_MASK )) return INPUT_UP ;
	if (!(JOY_DN_PIN  & JOY_DN_MASK )) return INPUT_DN ;
	if (!(JOY_LT_PIN  & JOY_LT_MASK )) return INPUT_LT ;
	if (!(JOY_RT_PIN  & JOY_RT_MASK )) return INPUT_RT ;
	return 0;
}

uint8_t input_getall(void) {
	uint8_t r = 0;
	if (!(VS_POS_PIN  & VS_POS_MASK )) r |= INPUT_VS ;
	if (!(JOY_CTR_PIN & JOY_CTR_MASK)) r |= INPUT_CTR;
	if (!(JOY_UP_PIN  & JOY_UP_MASK )) r |= INPUT_UP ;
	if (!(JOY_DN_PIN  & JOY_DN_MASK )) r |= INPUT_DN ;
	if (!(JOY_LT_PIN  & JOY_LT_MASK )) r |= INPUT_LT ;
	if (!(JOY_RT_PIN  & JOY_RT_MASK )) r |= INPUT_RT ;
	return r;
}
