/**********************************************************
 * PicoFS FAT Library for Arduino by Rebecca Bettencourt. *
 * Based on PetitFS by ChaN. MIT license.                 *
 **********************************************************/

#ifndef __PICOFS_DISKIO__
#define __PICOFS_DISKIO__

#include <Arduino.h>

// Status of disk functions.
#define STA_OK     0
#define STA_NOINIT 1
#define STA_NODISK 2

// Results of disk functions.
#define RES_OK     0
#define RES_ERROR  1
#define RES_NOTRDY 2
#define RES_PARERR 3

// Card types.
#define CT_MMC     1
#define CT_SD1     2
#define CT_SD2     4
#define CT_SDC     6
#define CT_BLOCK   8

uint8_t disk_initialize(void);
uint8_t disk_read(uint8_t * buf, uint32_t sector);
uint8_t disk_write(uint8_t * buf, uint32_t sector);

#endif
