#include <Arduino.h>
#include "spi.h"

SPIClass SPI;

void SPIClass::begin() {
	SPI_SS_PORT  |= SPI_SS_MASK  ; // digitalWrite(SS, HIGH);
	SPI_SS_DDR   |= SPI_SS_MASK  ; // pinMode(SS, OUTPUT);
	SPCR |= _BV(MSTR);
	SPCR |= _BV(SPE);
	SPI_SCK_DDR  |= SPI_SCK_MASK ; // pinMode(SCK, OUTPUT);
	SPI_MOSI_DDR |= SPI_MOSI_MASK; // pinMode(MOSI, OUTPUT);
}

void SPIClass::end() {
	SPCR &=~ _BV(SPE);
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
	if (bitOrder == LSBFIRST) SPCR |= _BV(DORD);
	else SPCR &=~ _BV(DORD);
}

void SPIClass::setDataMode(uint8_t mode) {
	SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

void SPIClass::setClockDivider(uint8_t rate) {
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

void SPIClass::readBlock(uint8_t * buf, uint16_t cnt) {
	if (!cnt) return;
	SPDR = 0xFF;
	cnt--;
	while (cnt) {
		while (!(SPSR & _BV(SPIF)));
		*buf = SPDR;
		SPDR = 0xFF;
		buf++;
		cnt--;
	}
	while (!(SPSR & _BV(SPIF)));
	*buf = SPDR;
}

void SPIClass::writeBlock(uint8_t * buf, uint16_t cnt) {
	if (!cnt) return;
	SPDR = *buf;
	buf++;
	cnt--;
	while (cnt) {
		while (!(SPSR & _BV(SPIF)));
		SPDR = *buf;
		buf++;
		cnt--;
	}
	while (!(SPSR & _BV(SPIF)));
}

void SPIClass::writeBlock16(uint16_t * buf, uint16_t cnt) {
	if (!cnt) return;
	SPDR = (*buf) >> 8;
	while (!(SPSR & _BV(SPIF)));
	SPDR = *buf;
	buf++;
	cnt--;
	while (cnt) {
		while (!(SPSR & _BV(SPIF)));
		SPDR = (*buf) >> 8;
		while (!(SPSR & _BV(SPIF)));
		SPDR = *buf;
		buf++;
		cnt--;
	}
	while (!(SPSR & _BV(SPIF)));
}

void SPIClass::writeRep16(uint8_t a, uint8_t b, uint16_t cnt) {
	if (!cnt) return;
	SPDR = a;
	while (!(SPSR & _BV(SPIF)));
	SPDR = b;
	cnt--;
	while (cnt) {
		while (!(SPSR & _BV(SPIF)));
		SPDR = a;
		while (!(SPSR & _BV(SPIF)));
		SPDR = b;
		cnt--;
	}
	while (!(SPSR & _BV(SPIF)));
}
