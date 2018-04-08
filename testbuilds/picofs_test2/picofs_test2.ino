#include <Arduino.h>
#include <SPI.h>
#include "picofs_diskio.h"
#include "picofs_pins.h"
#include "picofs.h"

FATFS fs = FATFS();
uint16_t ptr = 0;
uint16_t ptr2 = 0;
uint32_t sector;
uint32_t sector2;
uint8_t tmp[512];
uint32_t offsets[16];
uint32_t lengths[16];

void setup(void) {
  fs.init(true);
  Serial.begin(9600);
  Serial.println("ready");
}

void loop(void) {
  switch ((fs.buf[ptr] = Serial.read())) {
    case '\n':
    case '\r':
      fs.buf[ptr] = 0;
      switch (fs.buf[0]) {
        case '!':
          Serial.println((char *)(fs.buf + 1));
          break;
        case 'i':
          Serial.println(disk_initialize());
          break;
        case 'm':
          Serial.println(fs.mount());
          break;
        case 'o':
          strcpy((char *)tmp, (char *)fs.buf);
          Serial.println(fs.open((char *)(tmp + 1)));
          break;
        case 's':
          sector = 0;
          for (ptr = 1; fs.buf[ptr]; ptr++) {
            if (fs.buf[ptr] >= '0' && fs.buf[ptr] <= '9') { sector <<= 4; sector |= (fs.buf[ptr] - '0'); }
            if (fs.buf[ptr] >= 'A' && fs.buf[ptr] <= 'F') { sector <<= 4; sector |= (fs.buf[ptr] - 'A' + 10); }
            if (fs.buf[ptr] >= 'a' && fs.buf[ptr] <= 'f') { sector <<= 4; sector |= (fs.buf[ptr] - 'a' + 10); }
          }
          Serial.println(fs.seek(sector));
          break;
        case 'r':
          sector = 0;
          for (ptr = 1; fs.buf[ptr]; ptr++) {
            if (fs.buf[ptr] >= '0' && fs.buf[ptr] <= '9') { sector <<= 4; sector |= (fs.buf[ptr] - '0'); }
            if (fs.buf[ptr] >= 'A' && fs.buf[ptr] <= 'F') { sector <<= 4; sector |= (fs.buf[ptr] - 'A' + 10); }
            if (fs.buf[ptr] >= 'a' && fs.buf[ptr] <= 'f') { sector <<= 4; sector |= (fs.buf[ptr] - 'a' + 10); }
          }
          if ((ptr == 1) ? fs.read() : disk_read(fs.buf, sector)) {
            Serial.println("error");
          } else {
            for (sector = 0; sector < 512; sector += 16) {
              for (ptr = 0; ptr < 16; ptr++) Serial.print(hex(fs.buf[sector | ptr]));
              for (ptr = 0; ptr < 16; ptr++) Serial.print(chr(fs.buf[sector | ptr]));
              Serial.println();
            }
          }
          break;
        case 'l':
          sector = 0;
          for (ptr = 1; fs.buf[ptr]; ptr++) {
            if (fs.buf[ptr] >= '0' && fs.buf[ptr] <= '9') { sector <<= 4; sector |= (fs.buf[ptr] - '0'); }
            if (fs.buf[ptr] >= 'A' && fs.buf[ptr] <= 'F') { sector <<= 4; sector |= (fs.buf[ptr] - 'A' + 10); }
            if (fs.buf[ptr] >= 'a' && fs.buf[ptr] <= 'f') { sector <<= 4; sector |= (fs.buf[ptr] - 'a' + 10); }
          }
          if ((ptr == 1) ? fs.read() : disk_read(fs.buf, sector)) {
            Serial.println("error");
          } else {
            for (ptr = 0; ptr < 512; ptr += 32) {
            	if (!fs.buf[ptr]) break;
            	if (fs.buf[ptr] == 0xE5) continue;
            	if (fs.buf[ptr] == 0x05) fs.buf[ptr] = 0xE5;
            	if (fs.buf[ptr + 0x0B] & 0x0A) continue;
            	for (ptr2 = 0; ptr2 < 8; ptr2++) tmp[ptr2] = fs.buf[ptr + ptr2];
            	tmp[8] = ' ';
            	for (ptr2 = 8; ptr2 < 11; ptr2++) tmp[ptr2 + 1] = fs.buf[ptr + ptr2];
            	tmp[12] = 0;
            	sector   = fs.buf[ptr + 0x15]; sector  <<= 8;
            	sector  |= fs.buf[ptr + 0x14]; sector  <<= 8;
            	sector  |= fs.buf[ptr + 0x1B]; sector  <<= 8;
            	sector  |= fs.buf[ptr + 0x1A];
            	sector2  = fs.buf[ptr + 0x1F]; sector2 <<= 8;
            	sector2 |= fs.buf[ptr + 0x1E]; sector2 <<= 8;
            	sector2 |= fs.buf[ptr + 0x1D]; sector2 <<= 8;
            	sector2 |= fs.buf[ptr + 0x1C];
            	Serial.print((char *)tmp);
            	Serial.print("\t");
            	Serial.print(sector, HEX);
            	Serial.print("\t");
            	Serial.println(sector2, HEX);
            }
          }
          break;
        case 'w':
          sector = 0;
          for (ptr = 1; fs.buf[ptr]; ptr++) {
            if (fs.buf[ptr] >= '0' && fs.buf[ptr] <= '9') { sector <<= 4; sector |= (fs.buf[ptr] - '0'); }
            if (fs.buf[ptr] >= 'A' && fs.buf[ptr] <= 'F') { sector <<= 4; sector |= (fs.buf[ptr] - 'A' + 10); }
            if (fs.buf[ptr] >= 'a' && fs.buf[ptr] <= 'f') { sector <<= 4; sector |= (fs.buf[ptr] - 'a' + 10); }
          }
          Serial.println("writing");
          for (ptr2 = 0; ptr2 < 512; ptr2++) {
            do {
              tmp[0] = Serial.read();
              if (tmp[0] == '?') Serial.println(ptr2);
              tmp[0] = dec(tmp[0]);
            } while (tmp[0] == 0xFF);
            do {
              tmp[1] = Serial.read();
              if (tmp[1] == '?') Serial.println(ptr2);
              tmp[1] = dec(tmp[1]);
            } while (tmp[1] == 0xFF);
            fs.buf[ptr2] = (tmp[0] << 4) | tmp[1];
          }
          if ((ptr == 1) ? fs.write() : disk_write(fs.buf, sector)) {
            Serial.println("error");
          } else {
            Serial.println("written");
          }
          break;
        case 'f':
          ptr = fs.frags(offsets, lengths, 16);
          if (ptr < 1 || ptr > 16) {
            Serial.println("error");
          } else {
            for (ptr2 = 0; ptr2 < ptr; ptr2++) {
              Serial.print(offsets[ptr2], HEX);
              Serial.print("\t");
              Serial.println(lengths[ptr2], HEX);
            }
          }
          break;
        case 'c':
          fs.close();
          break;
      }
      ptr = 0;
      break;
    case 0xFF:
      break;
    default:
      if (ptr < 0xFF) ptr++;
      break;
  }
}

char * hex(uint8_t v) {
  tmp[0] = '0' + (v >> 4); if (tmp[0] > '9') tmp[0] += 7;
  tmp[1] = '0' + (v & 15); if (tmp[1] > '9') tmp[1] += 7;
  tmp[2] = ' ';
  tmp[3] = 0;
  return (char *)tmp;
}

char * chr(uint8_t v) {
  tmp[0] = (v < 32) ? '.' : (v < 127) ? v : '.';
  tmp[1] = 0;
  return (char *)tmp;
}

uint8_t dec(uint8_t ch) {
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
  if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
  return 255;
}

void strcpy(char * dest, char * src) {
  while (*dest++ = *src++);
}
