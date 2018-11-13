#ifndef __SPRITELET_PINS__
#define __SPRITELET_PINS__

#include <Arduino.h>

#define VS_GND       5
#define VS_GND_DDR   DDRC
#define VS_GND_PORT  PORTC
#define VS_GND_PIN   PINC
#define VS_GND_MASK  0x40

#define VS_POS       7
#define VS_POS_DDR   DDRE
#define VS_POS_PORT  PORTE
#define VS_POS_PIN   PINE
#define VS_POS_MASK  0x40

#define JOY_COM      A0
#define JOY_COM_DDR  DDRF
#define JOY_COM_PORT PORTF
#define JOY_COM_PIN  PINF
#define JOY_COM_MASK 0x80

#define JOY_CTR      A1
#define JOY_CTR_DDR  DDRF
#define JOY_CTR_PORT PORTF
#define JOY_CTR_PIN  PINF
#define JOY_CTR_MASK 0x40

#define JOY_UP       A2
#define JOY_UP_DDR   DDRF
#define JOY_UP_PORT  PORTF
#define JOY_UP_PIN   PINF
#define JOY_UP_MASK  0x20

#define JOY_DN       A3
#define JOY_DN_DDR   DDRF
#define JOY_DN_PORT  PORTF
#define JOY_DN_PIN   PINF
#define JOY_DN_MASK  0x10

#define JOY_LT       A4
#define JOY_LT_DDR   DDRF
#define JOY_LT_PORT  PORTF
#define JOY_LT_PIN   PINF
#define JOY_LT_MASK  0x02

#define JOY_RT       A5
#define JOY_RT_DDR   DDRF
#define JOY_RT_PORT  PORTF
#define JOY_RT_PIN   PINF
#define JOY_RT_MASK  0x01

#endif
