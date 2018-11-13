/**********************************************************
 * PicoFS FAT Library for Arduino by Rebecca Bettencourt. *
 * Based on PetitFS by ChaN. MIT license.                 *
 **********************************************************/

#ifndef __PICOFS_PINS__
#define __PICOFS_PINS__

#include <Arduino.h>

// Chip select line for SD card.
#define SD_CS      12
#define SD_CS_DDR  DDRD
#define SD_CS_PORT PORTD
#define SD_CS_PIN  PIND
#define SD_CS_MASK 0x40

#endif
