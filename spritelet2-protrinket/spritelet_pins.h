#ifndef __SPRITELET_PINS__
#define __SPRITELET_PINS__

#include <Arduino.h>

#define VS_GND       4
#define VS_GND_DDR   DDRD
#define VS_GND_PORT  PORTD
#define VS_GND_PIN   PIND
#define VS_GND_MASK  0x10

#define VS_POS       3
#define VS_POS_DDR   DDRD
#define VS_POS_PORT  PORTD
#define VS_POS_PIN   PIND
#define VS_POS_MASK  0x08

#define JOY_COM      A0
#define JOY_COM_DDR  DDRC
#define JOY_COM_PORT PORTC
#define JOY_COM_PIN  PINC
#define JOY_COM_MASK 0x01

#define JOY_CTR      A1
#define JOY_CTR_DDR  DDRC
#define JOY_CTR_PORT PORTC
#define JOY_CTR_PIN  PINC
#define JOY_CTR_MASK 0x02

#define JOY_UP       A2
#define JOY_UP_DDR   DDRC
#define JOY_UP_PORT  PORTC
#define JOY_UP_PIN   PINC
#define JOY_UP_MASK  0x04

#define JOY_DN       A3
#define JOY_DN_DDR   DDRC
#define JOY_DN_PORT  PORTC
#define JOY_DN_PIN   PINC
#define JOY_DN_MASK  0x08

#define JOY_LT       A4
#define JOY_LT_DDR   DDRC
#define JOY_LT_PORT  PORTC
#define JOY_LT_PIN   PINC
#define JOY_LT_MASK  0x10

#define JOY_RT       A5
#define JOY_RT_DDR   DDRC
#define JOY_RT_PORT  PORTC
#define JOY_RT_PIN   PINC
#define JOY_RT_MASK  0x20

#endif
