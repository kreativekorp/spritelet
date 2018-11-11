#ifndef __SPRITELET_INPUT__
#define __SPRITELET_INPUT__

#include <Arduino.h>

#define INPUT_VS  0x01
#define INPUT_CTR 0x02
#define INPUT_UP  0x04
#define INPUT_DN  0x08
#define INPUT_LT  0x10
#define INPUT_RT  0x20

void input_init(void);
uint8_t input_get(void);
uint8_t input_getall(void);

#endif
