#include <Arduino.h>
#include <SPI.h>
#include "picofs_diskio.h"
#include "picofs_pins.h"

uint8_t buf[512];
uint16_t ptr = 0;
uint32_t sector;
uint8_t tmp[4];

void setup(void) {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);
  Serial.begin(9600);
  Serial.println("ready");
}

void loop(void) {
  switch ((buf[ptr] = Serial.read())) {
    case '\n':
    case '\r':
      buf[ptr] = 0;
      switch (buf[0]) {
        case '!':
          Serial.println((char *)(buf + 1));
          break;
        case 'i':
          Serial.println(disk_initialize());
          break;
        case 'r':
          sector = 0;
          for (ptr = 1; buf[ptr]; ptr++) {
            if (buf[ptr] >= '0' && buf[ptr] <= '9') { sector <<= 4; sector |= (buf[ptr] - '0'); }
            if (buf[ptr] >= 'A' && buf[ptr] <= 'F') { sector <<= 4; sector |= (buf[ptr] - 'A' + 10); }
            if (buf[ptr] >= 'a' && buf[ptr] <= 'f') { sector <<= 4; sector |= (buf[ptr] - 'a' + 10); }
          }
          if (disk_read(buf, sector)) {
            Serial.println("error");
          } else {
            for (sector = 0; sector < 512; sector += 16) {
              for (ptr = 0; ptr < 16; ptr++) Serial.print(hex(buf[sector | ptr]));
              for (ptr = 0; ptr < 16; ptr++) Serial.print(chr(buf[sector | ptr]));
              Serial.println();
            }
          }
          break;
        case 'w':
          sector = 0;
          for (ptr = 1; buf[ptr]; ptr++) {
            if (buf[ptr] >= '0' && buf[ptr] <= '9') { sector <<= 4; sector |= (buf[ptr] - '0'); }
            if (buf[ptr] >= 'A' && buf[ptr] <= 'F') { sector <<= 4; sector |= (buf[ptr] - 'A' + 10); }
            if (buf[ptr] >= 'a' && buf[ptr] <= 'f') { sector <<= 4; sector |= (buf[ptr] - 'a' + 10); }
          }
          Serial.println("writing");
          for (ptr = 0; ptr < 512; ptr++) {
            do {
              tmp[0] = Serial.read();
              if (tmp[0] == '?') Serial.println(ptr);
              tmp[0] = dec(tmp[0]);
            } while (tmp[0] == 0xFF);
            do {
              tmp[1] = Serial.read();
              if (tmp[1] == '?') Serial.println(ptr);
              tmp[1] = dec(tmp[1]);
            } while (tmp[1] == 0xFF);
            buf[ptr] = (tmp[0] << 4) | tmp[1];
          }
          if (disk_write(buf, sector)) {
            Serial.println("error");
          } else {
            Serial.println("written");
          }
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
