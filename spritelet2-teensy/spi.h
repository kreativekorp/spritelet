#ifndef __SPI_H__
#define __SPI_H__

#include <Arduino.h>
#include <SPI.h>

#define SPI_begin() do { SPISettings s(12000000, MSBFIRST, SPI_MODE0); SPI.begin(); SPI.beginTransaction(s); } while (0)
#define SPI_end() SPI.end()

#define SPI_transfer(x) SPI.transfer(x)
#define SPI_read() SPI.transfer(0xFF)
#define SPI_write(x) SPI.transfer(x)
#define SPI_skip() SPI.transfer(0xFF)

void SPI_readBlock(uint8_t * buf, uint16_t cnt);
void SPI_writeBlock(uint8_t * buf, uint16_t cnt);
void SPI_writeBlock16(uint16_t * buf, uint16_t cnt);
void SPI_writeRep16(uint8_t a, uint8_t b, uint16_t cnt);

#endif
