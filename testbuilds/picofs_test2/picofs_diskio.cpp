/**********************************************************
 * PicoFS FAT Library for Arduino by Rebecca Bettencourt. *
 * Based on PetitFS by ChaN. MIT license.                 *
 **********************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "picofs_diskio.h"
#include "picofs_pins.h"

#define CMD0   0x40
#define CMD1   0x41
#define CMD8   0x48
#define CMD16  0x50
#define CMD17  0x51
#define CMD24  0x58
#define CMD55  0x77
#define CMD58  0x7A
#define ACMD41 0xE9

static uint8_t send_cmd(uint8_t cmd, uint32_t arg) {
	uint8_t res, i;
	if (cmd & 0x80) {
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
		cmd &= 0x7F;
	}
	SPI.transfer(0xFF);
	SPI.transfer(cmd);
	SPI.transfer(arg >> 24);
	SPI.transfer(arg >> 16);
	SPI.transfer(arg >> 8);
	SPI.transfer(arg);
	SPI.transfer((cmd == CMD0) ? 0x95 : (cmd == CMD8) ? 0x87 : 0x01);
	for (i = 0; ((res = SPI.transfer(0xFF)) & 0x80) && i < 10; i++);
	return res;
}

static uint8_t card_type;

uint8_t disk_initialize(void) {
	uint8_t i, cmd, ocr[4];
	uint16_t wait;
	card_type = 0;
	SD_CS_PORT |= SD_CS_MASK;
	for (i = 0; i < 10; i++) SPI.transfer(0xFF);
	SD_CS_PORT &=~ SD_CS_MASK;
	if (send_cmd(CMD0, 0) == 1) {
		if (send_cmd(CMD8, 0x1AA) == 1) {
			for (i = 0; i < 4; i++) ocr[i] = SPI.transfer(0xFF);
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
				wait = 10000;
				while (wait && send_cmd(ACMD41, 0x40000000)) {
					delayMicroseconds(100);
					wait--;
				}
				if (wait && !send_cmd(CMD58, 0)) {
					for (i = 0; i < 4; i++) ocr[i] = SPI.transfer(0xFF);
					card_type = (ocr[0] & 0x40) ? (CT_SD2 | CT_BLOCK) : CT_SD2;
				}
			}
		} else {
			if (send_cmd(ACMD41, 0) <= 1) {
				card_type = CT_SD1;
				cmd = ACMD41;
			} else {
				card_type = CT_MMC;
				cmd = CMD1;
			}
			wait = 10000;
			while (wait && send_cmd(cmd, 0)) {
				delayMicroseconds(100);
				wait--;
			}
			if (!wait || send_cmd(CMD16, 512)) {
				card_type = 0;
			}
		}
	}
	SD_CS_PORT |= SD_CS_MASK;
	return card_type ? STA_OK : STA_NOINIT;
}

uint8_t disk_read(uint8_t * buf, uint32_t sector) {
	uint8_t res, rc;
	uint16_t wait, bc;
	res = RES_ERROR;
	if (!(card_type & CT_BLOCK)) sector <<= 9;
	SD_CS_PORT &=~ SD_CS_MASK;
	if (!send_cmd(CMD17, sector)) {
		for (wait = 40000; ((rc = SPI.transfer(0xFF)) == 0xFF) && wait; wait--);
		if (rc == 0xFE) {
			for (bc = 0; bc < 512; bc++) *buf++ = SPI.transfer(0xFF);
			SPI.transfer(0xFF);
			SPI.transfer(0xFF);
			res = RES_OK;
		}
	}
	SD_CS_PORT |= SD_CS_MASK;
	return res;
}

uint8_t disk_write(uint8_t * buf, uint32_t sector) {
	uint8_t res;
	uint16_t wait, bc;
	res = RES_ERROR;
	if (!(card_type & CT_BLOCK)) sector <<= 9;
	SD_CS_PORT &=~ SD_CS_MASK;
	if (!send_cmd(CMD24, sector)) {
		SPI.transfer(0xFF);
		SPI.transfer(0xFE);
		for (bc = 0; bc < 512; bc++) SPI.transfer(*buf++);
		SPI.transfer(0);
		SPI.transfer(0);
		if ((SPI.transfer(0xFF) & 0x1F) == 0x05) {
			wait = 5000;
			while (wait && (SPI.transfer(0xFF) != 0xFF)) {
				delayMicroseconds(100);
				wait--;
			}
			if (wait) res = RES_OK;
		}
	}
	SD_CS_PORT |= SD_CS_MASK;
	return res;
}