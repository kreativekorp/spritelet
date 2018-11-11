#include <Arduino.h>
#include <SPI.h>
#include "spi.h"

void SPI_readBlock(uint8_t * buf, uint16_t cnt) {
  while (cnt) {
    *buf = SPI_read();
    buf++;
    cnt--;
  }
}

void SPI_writeBlock(uint8_t * buf, uint16_t cnt) {
  while (cnt) {
    SPI_write(*buf);
    buf++;
    cnt--;
  }
}

void SPI_writeBlock16(uint16_t * buf, uint16_t cnt) {
  while (cnt) {
    SPI_write((*buf) >> 8);
    SPI_write(*buf);
    buf++;
    cnt--;
  }
}

void SPI_writeRep16(uint8_t a, uint8_t b, uint16_t cnt) {
  while (cnt) {
    SPI_write(a);
    SPI_write(b);
    cnt--;
  }
}
