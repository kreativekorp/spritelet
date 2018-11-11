#include <Arduino.h>
#include "spritelet_input.h"
#include "spritelet_pins.h"

void input_init(void) {
  pinMode(VS_GND , OUTPUT); digitalWrite(VS_GND , LOW );
  pinMode(VS_POS , INPUT ); digitalWrite(VS_POS , HIGH);
  pinMode(JOY_COM, OUTPUT); digitalWrite(JOY_COM, LOW );
  pinMode(JOY_CTR, INPUT ); digitalWrite(JOY_CTR, HIGH);
  pinMode(JOY_UP , INPUT ); digitalWrite(JOY_UP , HIGH);
  pinMode(JOY_DN , INPUT ); digitalWrite(JOY_DN , HIGH);
  pinMode(JOY_LT , INPUT ); digitalWrite(JOY_LT , HIGH);
  pinMode(JOY_RT , INPUT ); digitalWrite(JOY_RT , HIGH);
}

uint8_t input_get(void) {
  if (!digitalRead(VS_POS )) return INPUT_VS ;
  if (!digitalRead(JOY_CTR)) return INPUT_CTR;
  if (!digitalRead(JOY_UP )) return INPUT_UP ;
  if (!digitalRead(JOY_DN )) return INPUT_DN ;
  if (!digitalRead(JOY_LT )) return INPUT_LT ;
  if (!digitalRead(JOY_RT )) return INPUT_RT ;
  return 0;
}

uint8_t input_getall(void) {
  uint8_t r = 0;
  if (!digitalRead(VS_POS )) r |= INPUT_VS ;
  if (!digitalRead(JOY_CTR)) r |= INPUT_CTR;
  if (!digitalRead(JOY_UP )) r |= INPUT_UP ;
  if (!digitalRead(JOY_DN )) r |= INPUT_DN ;
  if (!digitalRead(JOY_LT )) r |= INPUT_LT ;
  if (!digitalRead(JOY_RT )) r |= INPUT_RT ;
  return r;
}
