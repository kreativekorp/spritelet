#ifndef __SPI_H__
#define __SPI_H__

#include <Arduino.h>

#define SPI_SS        SS
#define SPI_SS_DDR    DDRB
#define SPI_SS_PORT   PORTB
#define SPI_SS_PIN    PINB
#define SPI_SS_MASK   0x01

#define SPI_MOSI      MOSI
#define SPI_MOSI_DDR  DDRB
#define SPI_MOSI_PORT PORTB
#define SPI_MOSI_PIN  PINB
#define SPI_MOSI_MASK 0x04

#define SPI_MISO      MISO
#define SPI_MISO_DDR  DDRB
#define SPI_MISO_PORT PORTB
#define SPI_MISO_PIN  PINB
#define SPI_MISO_MASK 0x08

#define SPI_SCK       SCK
#define SPI_SCK_DDR   DDRB
#define SPI_SCK_PORT  PORTB
#define SPI_SCK_PIN   PINB
#define SPI_SCK_MASK  0x02

#define SPI_CLOCK_DIV4   0x00
#define SPI_CLOCK_DIV16  0x01
#define SPI_CLOCK_DIV64  0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2   0x04
#define SPI_CLOCK_DIV8   0x05
#define SPI_CLOCK_DIV32  0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK    0x0C
#define SPI_CLOCK_MASK   0x03
#define SPI_2XCLOCK_MASK 0x01

class SPIClass {
  public:
    static void begin();
    static void end();

    static void setBitOrder(uint8_t bitOrder);
    static void setDataMode(uint8_t dataMode);
    static void setClockDivider(uint8_t clockDivider);

    static inline uint8_t transfer(uint8_t data);
    static inline uint8_t read();
    static inline void write(uint8_t data);
    static inline void skip();

    static void readBlock(uint8_t * buf, uint16_t cnt);
    static void writeBlock(uint8_t * buf, uint16_t cnt);
    static void writeBlock16(uint16_t * buf, uint16_t cnt);
    static void writeRep16(uint8_t a, uint8_t b, uint16_t cnt);

    static inline void attachInterrupt();
    static inline void detachInterrupt();
};

extern SPIClass SPI;

uint8_t SPIClass::transfer(uint8_t data) {
  SPDR = data;
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

uint8_t SPIClass::read() {
  SPDR = 0xFF;
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

void SPIClass::write(uint8_t data) {
  SPDR = data;
  while (!(SPSR & _BV(SPIF)));
}

void SPIClass::skip() {
  SPDR = 0xFF;
  while (!(SPSR & _BV(SPIF)));
}

void SPIClass::attachInterrupt() {
  SPCR |= _BV(SPIE);
}

void SPIClass::detachInterrupt() {
  SPCR &=~ _BV(SPIE);
}

#endif
