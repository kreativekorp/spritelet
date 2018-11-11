#include <Arduino.h>
#include <avr/pgmspace.h>
#include "carousel.h"
#include "picofs.h"
#include "spritelet_bitmap.h"
#include "spritelet_font.h"
#include "spritelet_input.h"
#include "spritelet_util.h"
#include "st7735.h"

extern ST7735 tft;
extern FATFS fs;

static uint16_t menu_count;
static uint16_t menu_start;
static uint16_t menu_index;
static uint16_t menu_sector;
static uint16_t advmenu_start;
static uint16_t advmenu_index;
static uint32_t advmenu_timer;

static const char * PROGMEM menu_items[] = { "Cycle Through", "Randomize", "Home" };
static const char * PROGMEM advmenu_title = "Set Time Delay";
static const char * PROGMEM advmenu_cancel = "Cancel";
static const char * PROGMEM toc_path = "SPRITES/SPRITES.STC";
static const char * PROGMEM toc_not_found[] = { "Could not find", "SPRITES folder." };

static void render_menu(uint8_t mask) {
  uint8_t m; uint16_t y, i, clr, ptr, sect;
  // Draw Header
  if (mask & 0x80) {
    tft.fillRect(0, 0, 128, 16, 0x000F);
    if (menu_sector != 0) { fs.seek(menu_sector = 0); fs.read(); }
    tft_drawString(4, 2, (char *)(fs.buf + 32), 0x000F, 0xFFFF);
  }
  // Draw Menu Items
  for (m = 0x40, y = 16, i = menu_start; m; m >>= 1, y += 16, i++) {
    if (mask & m) {
      clr = ((i == menu_index) ? 0xC000 : 0x0000);
      tft.fillRect(0, y, 128, 16, clr);
      if (i < menu_count) {
        ptr = ((((i + 1) & 0x07) << 6) | 32);
        sect = ((i + 1) >> 3);
        if (menu_sector != sect) { fs.seek(menu_sector = sect); fs.read(); }
        tft_drawString(4, y + 2, (char *)(fs.buf + ptr), clr, 0xFFFF);
      } else if ((ptr = i - menu_count) < 3) {
        tft_drawString(4, y + 2, (char *)menu_items[ptr], clr, 0xFFFF);
      }
    }
  }
}

static void render_advtime_menu(uint8_t mask) {
  uint8_t m; uint16_t y, i, clr;
  // Draw Header
  if (mask & 0x80) {
    tft.fillRect(0, 0, 128, 16, 0x000F);
    tft_drawString(4, 2, (char *)advmenu_title, 0x000F, 0xFFFF);
  }
  // Draw Menu Items
  for (m = 0x40, y = 16, i = advmenu_start; m; m >>= 1, y += 16, i++) {
    if (mask & m) {
      clr = ((i == advmenu_index) ? 0xC000 : 0x0000);
      tft.fillRect(0, y, 128, 16, clr);
      if (i == 0) {
        tft_drawString(4, y + 2, (char *)advmenu_cancel, clr, 0xFFFF);
      } else if (i <= 12) {
        char str[4];
        uint16_t secs = i * 5;
        if (secs < 10) {
          str[0] = '0' + secs;
          str[1] = 's';
          str[2] = 0;
        } else {
          str[0] = '0' + (secs / 10);
          str[1] = '0' + (secs % 10);
          str[2] = 's';
          str[3] = 0;
        }
        tft_drawString(4, y + 2, str, clr, 0xFFFF);
      }
    }
  }
}

static uint8_t advtime_menu() {
  uint8_t in_menu = 1, j;
  advmenu_index /= 5000;
  render_advtime_menu(0xFF);
  while (in_menu) {
    if ((j = input_get())) {
      switch (j) {
        case INPUT_UP:
        case INPUT_LT:
          if (advmenu_index > 0) {
            advmenu_index--;
            if (advmenu_index < advmenu_start) {
              advmenu_start = advmenu_index;
              render_advtime_menu(0x7F);
            } else {
              render_advtime_menu(0x60 >> (advmenu_index - advmenu_start));
            }
          } else {
            advmenu_start = 6;
            advmenu_index = 12;
            render_advtime_menu(0x7F);
          }
          break;
        case INPUT_DN:
        case INPUT_RT:
          if (advmenu_index < 12) {
            advmenu_index++;
            if (advmenu_index > advmenu_start + 6) {
              advmenu_start = advmenu_index - 6;
              render_advtime_menu(0x7F);
            } else {
              render_advtime_menu(0xC0 >> (advmenu_index - advmenu_start));
            }
          } else {
            advmenu_start = 0;
            advmenu_index = 0;
            render_advtime_menu(0x7F);
          }
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
  advmenu_index *= 5000;
  return !!advmenu_index;
}

static uint8_t menu() {
  uint8_t in_menu = 1, j;
  render_menu(0xFF);
  while (in_menu) {
    if ((j = input_get())) {
      switch (j) {
        case INPUT_UP:
        case INPUT_LT:
          if (menu_index > 0) {
            menu_index--;
            if (menu_index < menu_start) {
              menu_start = menu_index;
              render_menu(0x7F);
            } else {
              render_menu(0x60 >> (menu_index - menu_start));
            }
          } else {
            menu_start = (menu_count > 4) ? (menu_count - 4) : 0;
            menu_index = menu_count + 2;
            render_menu(0x7F);
          }
          break;
        case INPUT_DN:
        case INPUT_RT:
          if (menu_index < menu_count + 2) {
            menu_index++;
            if (menu_index > menu_start + 6) {
              menu_start = menu_index - 6;
              render_menu(0x7F);
            } else {
              render_menu(0xC0 >> (menu_index - menu_start));
            }
          } else {
            menu_start = 0;
            menu_index = 0;
            render_menu(0x7F);
          }
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
      
      if (!in_menu) {
        if (menu_index < menu_count) {
          advmenu_start = 0;
          advmenu_index = 0;
          return 1;
        } else if (menu_index < menu_count + 2) {
          if (advtime_menu()) {
            return 1;
          } else {
            render_menu(0xFF);
            in_menu = 1;
          }
        } else {
          return 0;
        }
      }
    }
  }
}

static uint8_t render_video_callback() {
  uint8_t j = input_get();
  switch (j) {
    case INPUT_UP:
    case INPUT_DN:
      j = INPUT_CTR;
    case INPUT_CTR:
    case INPUT_LT:
    case INPUT_RT:
      delay(50);
      while (input_get());
      delay(50);
      return j;
  }
  if (advmenu_index && ((millis() - advmenu_timer) > advmenu_index)) {
    return INPUT_VS;
  } else {
    return 0;
  }
}

static uint8_t render_video(uint16_t i) {
  uint16_t ptr, sect;
  char path[32];
  char * src;
  char * dest;
  uint8_t res;
  
  // get path to SMF file
  ptr = (((i + 1) & 0x07) << 6);
  sect = ((i + 1) >> 3);
  if (menu_sector != sect) { fs.seek(menu_sector = sect); fs.read(); }
  for (src = (char *)(fs.buf + ptr), dest = path; *src; src++, dest++) *dest = *src;
  *dest = 0;
  fs.close();
  
  // play SMF file
  tft.fillScreen(0);
  advmenu_timer = millis();
  res = tft_drawSMF(path, render_video_callback);
  while (!res) res = render_video_callback();
  tft.fillScreen(0);
  
  // reopen menu
  fs.open((char *)toc_path);
  fs.read();
  menu_sector = 0;
  return res;
}

uint8_t carousel_setup(void) {
  if (fs.open((char *)toc_path) || fs.read()) {
    error_message(20, (char *)toc_not_found[0], 20, (char *)toc_not_found[1], -1);
    return 0;
  } else {
    menu_count = fs.buf[0]; menu_count <<= 8; menu_count |= fs.buf[1];
    menu_start = fs.buf[2]; menu_start <<= 8; menu_start |= fs.buf[3];
    menu_index = fs.buf[4]; menu_index <<= 8; menu_index |= fs.buf[5];
    advmenu_start = fs.buf[6]; advmenu_start <<= 8; advmenu_start |= fs.buf[7];
    advmenu_index = fs.buf[8]; advmenu_index <<= 8; advmenu_index |= fs.buf[9];
    menu_sector = 0;
    return 1;
  }
}

uint8_t carousel_loop(void) {
  uint16_t idx;
  uint8_t res;
  if (menu_index < menu_count) {
    idx = menu_index;
    res = render_video(idx);
  } else switch (menu_index - menu_count) {
    case 0:
      for (;;) {
        for (idx = 0; idx < menu_count; idx++) {
          res = render_video(idx);
          if (res != INPUT_VS) break;
        }
        if (res != INPUT_VS) break;
      }
      break;
    case 1:
      for (;;) {
        idx = random(menu_count);
        res = render_video(idx);
        if (res != INPUT_VS) break;
      }
      break;
    default:
      fs.close();
      return 0;
  }
  switch (res) {
    case INPUT_LT:
      menu_index = ((idx > 0) ? (idx - 1) : (menu_count - 1));
      menu_start = (menu_index > 3) ? (menu_index - 3) : 0;
      advmenu_start = 0;
      advmenu_index = 0;
      break;
    case INPUT_RT:
      menu_index = ((idx < (menu_count - 1)) ? (idx + 1) : 0);
      menu_start = (menu_index > 3) ? (menu_index - 3) : 0;
      advmenu_start = 0;
      advmenu_index = 0;
      break;
    default:
      if (!menu()) {
        fs.close();
        return 0;
      }
      break;
  }
  return 1;
}
