#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include "carousel.h"
#include "clouds.h"
#include "dx.h"
#include "picofs_diskio.h"
#include "picofs_pins.h"
#include "picofs.h"
#include "spi.h"
#include "spritelet_bitmap.h"
#include "spritelet_font.h"
#include "spritelet_input.h"
#include "spritelet_pins.h"
#include "spritelet_util.h"
#include "st7735_commands.h"
#include "st7735_config.h"
#include "st7735_init.h"
#include "st7735_pins.h"
#include "st7735.h"
#include "toasters.h"

ST7735 tft = ST7735();
FATFS fs = FATFS();
static uint8_t pgmidx = 0;

static const char * PROGMEM mount_failed[] = { "Please insert", "MicroSD card." };
static const char * PROGMEM defaults_path = "DEFAULTS.SYS";
static const char * PROGMEM home_menu_title = "Home";

#define HOME_MENU_ITEMS 4
static const char * PROGMEM home_menu_items[] = {
  "Clouds",
  "Carousel",
  "dX",
  "Toasters",
};

void setup(void) {
  randomSeed(analogRead(2));
  Serial.end();

  input_init();
  tft.init(true);
  tft.fillScreen(0);
  tft.setBacklight(1);

  fs.init(false);
  if (fs.mount()) {
    tft_drawString(25, 52, (char *)mount_failed[0], 0, -1);
    tft_drawString(25, 64, (char *)mount_failed[1], 0, -1);
    do { delay(500); } while (fs.mount());
    tft.fillScreen(0);
  }

  if (!fs.open((char *)defaults_path)) {
    if (!fs.read()) {
      pgmidx = fs.buf[0];
    }
    fs.close();
  }
}

void loop(void) {
  switch (pgmidx) {
    case 0: if (clouds_setup()) while (clouds_loop()); break;
    case 1: if (carousel_setup()) while (carousel_loop()); break;
    case 2: if (dx_setup()) while (dx_loop()); break;
    case 3: if (toasters_setup()) while (toasters_loop()); break;
  }

  home_menu();

  if (!fs.open((char *)defaults_path)) {
    uint16_t ptr;
    fs.buf[0] = pgmidx;
    for (ptr = 1; ptr < 512; ptr++) fs.buf[ptr] = -1;
    fs.write();
    fs.close();
  }
}

static void render_home_menu(uint8_t mask) {
  uint8_t m; uint16_t y, i, clr;
  // Draw Header
  if (mask & 0x80) {
    tft.fillRect(0, 0, 128, 16, 0x000F);
    tft_drawString(4, 2, (char *)home_menu_title, 0x000F, 0xFFFF);
  }
  // Draw Menu Items
  for (m = 0x40, y = 16, i = 0; i < HOME_MENU_ITEMS; m >>= 1, y += 16, i++) {
    if (mask & m) {
      clr = ((i == pgmidx) ? 0xC000 : 0x0000);
      tft.fillRect(0, y, 128, 16, clr);
      tft_drawString(4, y + 2, (char *)home_menu_items[i], clr, 0xFFFF);
    }
  }
}

static void home_menu() {
  uint8_t in_menu = 1, j;
  render_home_menu(0xFF);
  while (in_menu) {
    if ((j = input_get())) {
      switch (j) {
        case INPUT_UP:
        case INPUT_LT:
          pgmidx = (pgmidx > 0) ? (pgmidx - 1) : (HOME_MENU_ITEMS - 1);
          render_home_menu(0x7F);
          break;
        case INPUT_DN:
        case INPUT_RT:
          pgmidx = (pgmidx < (HOME_MENU_ITEMS - 1)) ? (pgmidx + 1) : 0;
          render_home_menu(0x7F);
          break;
        case INPUT_CTR:
          tft.fillScreen(0);
          in_menu = 0;
          break;
        default:
          continue;
      }
      delay(50);
      while (input_get());
      delay(50);
    }
  }
}
