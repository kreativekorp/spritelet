#include <Arduino.h>
#include <avr/pgmspace.h>
#include "dx.h"
#include "picofs.h"
#include "spritelet_bitmap.h"
#include "spritelet_font.h"
#include "spritelet_input.h"
#include "spritelet_util.h"
#include "st7735.h"

extern ST7735 tft;
extern FATFS fs;

static uint16_t imageIndex;
static uint16_t imageCount;
static uint16_t imageData;
static uint16_t diceIndex;
static uint16_t diceCount;
static uint16_t diceData;
static uint16_t rollIndex;
static uint16_t rollCount;
static uint16_t rollData;
static uint16_t layoutIndex;
static uint16_t layoutCount;
static uint16_t layoutData;
static uint16_t mem;
static uint16_t currCount;
static uint16_t orderType;
static uint16_t currLayout;

#define MAX_DICE 64
static uint16_t currOrder[MAX_DICE];
static uint16_t currValue[MAX_DICE];



/* ---- STRINGS ---- */

static const char * const PROGMEM dx_path = "DICE.4DX";
static const char * const PROGMEM dx_not_found = "Cannot find 4DX.";
static const char * const PROGMEM main_menu_title = "dX";
static const char * const PROGMEM ok_menu_item = "OK";
static const char * const PROGMEM cancel_menu_item = "Cancel";
static const char * const PROGMEM smenu_items[] = { "Min. Value:", "Max. Value:", "Num. Sides:" };
static const char * const PROGMEM error_messages[] = { "No more room!", "Not editable!", "Nothing to remove!" };

#define MAIN_MENU_ITEMS 9
static const char * const PROGMEM main_menu_items[] = {
	"Roll",
	"Load Preset",
	"Replace",
	"Add",
	"Edit",
	"Remove",
	"Set Order",
	"Set Layout",
	"Home",
};

#define SET_ORDER_MENU_ITEMS 3
static const char * const PROGMEM set_order_menu_items[] = {
	"Normal",
	"Reverse",
	"Random",
};



/* ---- UTILITIES ---- */

static uint8_t no_animation() {
	return 42;
}

static void render_image(uint16_t image, int16_t x, int16_t y, uint16_t size) {
	uint32_t s = imageData; s += image; s <<= 7; s += size >> 9;
	tft_drawGCI(s, size & 0x1FF, x, y, no_animation);
}

static uint16_t dx_seek(uint16_t base, uint16_t index, uint16_t sect, uint16_t offs) {
	uint32_t s = base; s += index; s <<= 7; s += sect; s += offs >> 9;
	fs.seek(s); return offs & 0x1FF;
}

static void render_dec(int16_t x, int16_t y, uint16_t value, uint16_t bg, uint16_t fg) {
	char s[6]; s[5] = 0;
	if (value) { s[4] = '0' + value % 10; value /= 10; } else { s[4] = '0'; }
	if (value) { s[3] = '0' + value % 10; value /= 10; } else { s[3] = ' '; }
	if (value) { s[2] = '0' + value % 10; value /= 10; } else { s[2] = ' '; }
	if (value) { s[1] = '0' + value % 10; value /= 10; } else { s[1] = ' '; }
	if (value) { s[0] = '0' + value % 10;              } else { s[0] = ' '; }
	tft_drawString(x, y, s, bg, fg);
}



/* ---- DX CORE ---- */

static uint8_t load_4dx() {
	uint32_t seed;
	if (fs.open((char *)dx_path) || fs.read()) return 0;

	imageIndex  = fs.buf[ 0]; imageIndex  <<= 8; imageIndex  |= fs.buf[ 1];
	imageCount  = fs.buf[ 2]; imageCount  <<= 8; imageCount  |= fs.buf[ 3];
	imageData   = fs.buf[ 4]; imageData   <<= 8; imageData   |= fs.buf[ 5];
	diceIndex   = fs.buf[ 8]; diceIndex   <<= 8; diceIndex   |= fs.buf[ 9];
	diceCount   = fs.buf[10]; diceCount   <<= 8; diceCount   |= fs.buf[11];
	diceData    = fs.buf[12]; diceData    <<= 8; diceData    |= fs.buf[13];
	rollIndex   = fs.buf[16]; rollIndex   <<= 8; rollIndex   |= fs.buf[17];
	rollCount   = fs.buf[18]; rollCount   <<= 8; rollCount   |= fs.buf[19];
	rollData    = fs.buf[20]; rollData    <<= 8; rollData    |= fs.buf[21];
	layoutIndex = fs.buf[24]; layoutIndex <<= 8; layoutIndex |= fs.buf[25];
	layoutCount = fs.buf[26]; layoutCount <<= 8; layoutCount |= fs.buf[27];
	layoutData  = fs.buf[28]; layoutData  <<= 8; layoutData  |= fs.buf[29];
	mem         = fs.buf[32]; mem         <<= 8; mem         |= fs.buf[33];

	dx_seek(mem, 0, 0, 0); if (fs.read()) return 0;

	currCount   = fs.buf[ 0]; currCount   <<= 8; currCount   |= fs.buf[ 1];
	orderType   = fs.buf[ 2]; orderType   <<= 8; orderType   |= fs.buf[ 3];
	currLayout  = fs.buf[12]; currLayout  <<= 8; currLayout  |= fs.buf[13];
	seed        = fs.buf[14]; seed        <<= 8; seed        |= fs.buf[15];

	seed <<= 16;
	seed |= random(65536);
	randomSeed(seed);
	return 1;
}

static void order_dice(uint8_t first) {
	uint16_t i, j, k, tmp;
	switch (orderType) {
		case 0:
			if (first) {
				for (i = 0; i < currCount; i++) {
					currOrder[i] = i;
				}
			}
			break;
		case 1:
			if (first) {
				for (i = 0, j = currCount - 1; i < currCount; i++, j--) {
					currOrder[i] = j;
				}
			}
			break;
		case 2:
			if (first) {
				for (i = 0; i < currCount; i++) {
					currOrder[i] = i;
				}
			}
			for (i = 0, j = currCount; i < currCount; i++, j--) {
				k = i + random(j);
				tmp = currOrder[i];
				currOrder[i] = currOrder[k];
				currOrder[k] = tmp;
			}
			break;
	}
}

static void roll_dice() {
	uint16_t index, type, sides, maxs;
	for (index = 0; index < currCount; index++) {
		dx_seek(mem, 0, index + 1, 0); fs.read();
		type  = fs.buf[ 2]; type  <<= 8; type  |= fs.buf[ 3];
		sides = fs.buf[ 8]; sides <<= 8; sides |= fs.buf[ 9];
		maxs  = fs.buf[14]; maxs  <<= 8; maxs  |= fs.buf[15];
		switch (type) {
			case 0:         currValue[index] = random(maxs ); break;
			case 1: case 2: currValue[index] = random(sides); break;
		}
	}
}

static void render_dice_digit(uint16_t dice, uint8_t place, uint8_t value, int16_t x, int16_t y, uint16_t size) {
	uint16_t ptr, xoffs, image;
	ptr = place; ptr <<= 4; ptr += value; ptr <<= 2; ptr += size;
	ptr = dx_seek(diceData, dice, 0, ptr); fs.read();
	xoffs = fs.buf[ptr++]; xoffs <<= 8; xoffs |= fs.buf[ptr++];
	image = fs.buf[ptr++]; image <<= 8; image |= fs.buf[ptr++];
	render_image(image, x + xoffs, y, size);
}

static void render_dice() {
	uint16_t index, dice, type, minv, basev;
	uint16_t ptr, x, y, size, value, image;
	for (index = 0; index < currCount; index++) {
		dx_seek(mem, 0, index + 1, 0); fs.read();
		dice  = fs.buf[ 0]; dice  <<= 8; dice  |= fs.buf[ 1];
		type  = fs.buf[ 2]; type  <<= 8; type  |= fs.buf[ 3];
		minv  = fs.buf[ 6]; minv  <<= 8; minv  |= fs.buf[ 7];
		basev = fs.buf[10]; basev <<= 8; basev |= fs.buf[11];
		ptr = dx_seek(layoutData, currLayout, currCount, currOrder[index] * 6 + 2); fs.read();
		x    = fs.buf[ptr++]; x    <<= 8; x    |= fs.buf[ptr++];
		y    = fs.buf[ptr++]; y    <<= 8; y    |= fs.buf[ptr++];
		size = fs.buf[ptr++]; size <<= 8; size |= fs.buf[ptr++];
		switch (type) {
			case 0: value = currValue[index]               ; break;
			case 1: value = currValue[index] + minv - basev; break;
			case 2: value = currValue[index] + minv        ; break;
		}
		switch (type) {
			case 0: case 1:
				ptr = dx_seek(diceData, dice, 2, value << 1); fs.read();
				image = fs.buf[ptr++]; image <<= 8; image |= fs.buf[ptr++];
				render_image(image, x, y, size);
				break;
			case 2:
				ptr = dx_seek(diceData, dice, 1, 8); fs.read();
				image = fs.buf[ptr++]; image <<= 8; image |= fs.buf[ptr++];
				render_image(image, x, y, size);
				if (value < 10) {
					render_dice_digit(dice, 1, value, x, y, size);
				} else if (value < 100) {
					render_dice_digit(dice, 2, (value / 10), x, y, size);
					render_dice_digit(dice, 3, (value % 10), x, y, size);
				} else if (value < 1000) {
					render_dice_digit(dice, 4, (value / 100), x, y, size);
					render_dice_digit(dice, 5, ((value / 10) % 10), x, y, size);
					render_dice_digit(dice, 6, (value % 10), x, y, size);
				} else if (value < 10000) {
					render_dice_digit(dice, 7, (value / 1000), x, y, size);
					render_dice_digit(dice, 8, ((value / 100) % 10), x, y, size);
					render_dice_digit(dice, 9, ((value / 10) % 10), x, y, size);
					render_dice_digit(dice, 10, (value % 10), x, y, size);
				}
				break;
		}
	}
}

static void render_amenu(uint8_t mask, char * title, char ** options, uint16_t count, uint16_t start, uint16_t index) {
	uint8_t m; uint16_t y, i, clr;
	// Draw Header
	if (mask & 0x80) {
		tft.fillRect(0, 0, 128, 16, 0x000F);
		tft_drawString(4, 2, title, 0x000F, 0xFFFF);
	}
	// Draw Menu Items
	for (m = 0x40, y = 16, i = start; m; m >>= 1, y += 16, i++) {
		if (mask & m) {
			clr = ((i == index) ? 0xC000 : 0x0000);
			tft.fillRect(0, y, 128, 16, clr);
			if (i < count) {
				tft_drawString(4, y + 2, options[i], clr, 0xFFFF);
			} else if (i == count) {
				tft_drawString(4, y + 2, (char *)cancel_menu_item, clr, 0xFFFF);
			}
		}
	}
}

static void render_rmenu(uint8_t mask, char * title, uint16_t base, uint16_t count, uint16_t start, uint16_t index) {
	uint8_t m; uint16_t y, i, clr, ptr;
	// Draw Header
	if (mask & 0x80) {
		tft.fillRect(0, 0, 128, 16, 0x000F);
		tft_drawString(4, 2, title, 0x000F, 0xFFFF);
	}
	// Draw Menu Items
	for (m = 0x40, y = 16, i = start; m; m >>= 1, y += 16, i++) {
		if (mask & m) {
			clr = ((i == index) ? 0xC000 : 0x0000);
			tft.fillRect(0, y, 128, 16, clr);
			if (i < count) {
				ptr = dx_seek(base, 0, (i >> 4), (((i & 0x0F) << 5) | 2)); fs.read();
				tft_drawString(4, y + 2, (char *)(fs.buf + ptr), clr, 0xFFFF);
			} else if (i == count) {
				tft_drawString(4, y + 2, (char *)cancel_menu_item, clr, 0xFFFF);
			}
		}
	}
}

static void render_dmenu(uint8_t mask, char * title, uint16_t index) {
	uint16_t ptr, x, y, size;
	if (mask & 0x80) {
		tft.fillRect(0, 0, 128, 16, 0x000F);
		tft_drawString(4, 2, title, 0x000F, 0xFFFF);
		tft.fillRect(0, 16, 128, 96, 0x0000);
	}
	if ((mask & 0x01) && (index != currCount)) {
		tft.fillRect(0, 112, 128, 16, 0x0000);
		tft_drawString(4, 114, (char *)cancel_menu_item, 0x0000, 0xFFFF);
	}
	render_dice();
	if (index < currCount) {
		ptr = dx_seek(layoutData, currLayout, currCount, index * 6 + 2); fs.read();
		x    = fs.buf[ptr++]; x    <<= 8; x    |= fs.buf[ptr++];
		y    = fs.buf[ptr++]; y    <<= 8; y    |= fs.buf[ptr++];
		size = fs.buf[ptr++]; size <<= 8; size |= fs.buf[ptr++];
		switch (size) {
			case 0x0100: size =  8; break;
			case 0x0200: size = 12; break;
			case 0x0400: size = 16; break;
			case 0x0800: size = 24; break;
			case 0x1000: size = 32; break;
			case 0x2000: size = 48; break;
			case 0x4000: size = 64; break;
			case 0x8000: size = 96; break;
		}
		tft.fillRect(x, y, size, 3, 0xC000);
		tft.fillRect(x, y, 3, size, 0xC000);
		tft.fillRect(x, y + size - 3, size, 3, 0xC000);
		tft.fillRect(x + size - 3, y, 3, size, 0xC000);
	}
	if ((mask & 0x01) && (index == currCount)) {
		tft.fillRect(0, 112, 128, 16, 0xC000);
		tft_drawString(4, 114, (char *)cancel_menu_item, 0xC000, 0xFFFF);
	}
}

static void render_smenu(uint8_t mask, char * title, uint16_t minv, uint16_t sides, uint16_t index) {
	uint16_t clr;
	// Draw Header
	if (mask & 0x80) {
		tft.fillRect(0, 0, 128, 16, 0x000F);
		tft_drawString(4, 2, title, 0x000F, 0xFFFF);
	}
	// Draw Menu Items
	if (mask & 0x40) {
		clr = ((index == 0) ? 0xC000 : 0x0000);
		tft.fillRect(0, 16, 128, 16, clr);
		tft_drawString(4, 18, (char *)smenu_items[0], clr, 0xFFFF);
		render_dec(94, 18, minv, clr, 0xFFFF);
	}
	if (mask & 0x20) {
		clr = ((index == 1) ? 0xC000 : 0x0000);
		tft.fillRect(0, 32, 128, 16, clr);
		tft_drawString(4, 34, (char *)smenu_items[1], clr, 0xFFFF);
		render_dec(94, 34, minv + sides - 1, clr, 0xFFFF);
	}
	if (mask & 0x10) {
		clr = ((index == 2) ? 0xC000 : 0x0000);
		tft.fillRect(0, 48, 128, 16, clr);
		tft_drawString(4, 50, (char *)smenu_items[2], clr, 0xFFFF);
		render_dec(94, 50, sides, clr, 0xFFFF);
	}
	if (mask & 0x08) {
		clr = ((index == 3) ? 0xC000 : 0x0000);
		tft.fillRect(0, 64, 128, 16, clr);
		tft_drawString(4, 66, (char *)ok_menu_item, clr, 0xFFFF);
	}
	if (mask & 0x04) {
		clr = ((index == 4) ? 0xC000 : 0x0000);
		tft.fillRect(0, 80, 128, 16, clr);
		tft_drawString(4, 82, (char *)cancel_menu_item, clr, 0xFFFF);
	}
	if (mask & 0x02) tft.fillRect(0,  96, 128, 16, 0x0000);
	if (mask & 0x01) tft.fillRect(0, 112, 128, 16, 0x0000);
}

static uint16_t amenu(char * title, char ** options, uint16_t count, uint16_t index) {
	uint16_t start = ((count <= 6 || index <= 3) ? 0 : (index < count - 3) ? (index - 3) : (count - 6));
	uint8_t in_menu = 1, j;
	render_amenu(0xFF, title, options, count, start, index);
	while (in_menu) {
		if ((j = input_get())) {
			switch (j) {
				case INPUT_UP:
					if (index > 0) {
						index--;
						if (index < start) {
							start = index;
							render_amenu(0x7F, title, options, count, start, index);
						} else {
							render_amenu(0x60 >> (index - start), title, options, count, start, index);
						}
						break;
					}
					// else fallthrough;
				case INPUT_LT:
					start = (count > 6) ? (count - 6) : 0;
					index = count;
					render_amenu(0x7F, title, options, count, start, index);
					break;
				case INPUT_DN:
				case INPUT_RT:
					if (index < count) {
						index++;
						if (index > start + 6) {
							start = index - 6;
							render_amenu(0x7F, title, options, count, start, index);
						} else {
							render_amenu(0xC0 >> (index - start), title, options, count, start, index);
						}
						break;
					}
					start = 0;
					index = 0;
					render_amenu(0x7F, title, options, count, start, index);
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
	return index;
}

static uint16_t rmenu(char * title, uint16_t base, uint16_t count, uint16_t index) {
	uint16_t start = ((count <= 6 || index <= 3) ? 0 : (index < count - 3) ? (index - 3) : (count - 6));
	uint8_t in_menu = 1, j;
	render_rmenu(0xFF, title, base, count, start, index);
	while (in_menu) {
		if ((j = input_get())) {
			switch (j) {
				case INPUT_UP:
					if (index > 0) {
						index--;
						if (index < start) {
							start = index;
							render_rmenu(0x7F, title, base, count, start, index);
						} else {
							render_rmenu(0x60 >> (index - start), title, base, count, start, index);
						}
						break;
					}
					// else fallthrough;
				case INPUT_LT:
					start = (count > 6) ? (count - 6) : 0;
					index = count;
					render_rmenu(0x7F, title, base, count, start, index);
					break;
				case INPUT_DN:
				case INPUT_RT:
					if (index < count) {
						index++;
						if (index > start + 6) {
							start = index - 6;
							render_rmenu(0x7F, title, base, count, start, index);
						} else {
							render_rmenu(0xC0 >> (index - start), title, base, count, start, index);
						}
						break;
					}
					start = 0;
					index = 0;
					render_rmenu(0x7F, title, base, count, start, index);
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
	return index;
}

static uint16_t dmenu(char * title, uint16_t index) {
	uint8_t in_menu = 1, j;
	uint16_t i;
	if (index < currCount) index = currOrder[index];
	render_dmenu(0xFF, title, index);
	while (in_menu) {
		if ((j = input_get())) {
			switch (j) {
				case INPUT_UP:
					if (index > 0) {
						index--;
						render_dmenu((index >= currCount - 1), title, index);
						break;
					}
					// else fallthrough;
				case INPUT_LT:
					index = currCount;
					render_dmenu(1, title, index);
					break;
				case INPUT_DN:
				case INPUT_RT:
					if (index < currCount) {
						index++;
						render_dmenu((index >= currCount), title, index);
						break;
					}
					index = 0;
					render_dmenu(1, title, index);
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
	if (index < currCount) {
		for (i = 0; i < currCount; i++) {
			if (currOrder[i] == index) {
				index = i;
				break;
			}
		}
	}
	return index;
}

static uint16_t smenu(char * title, uint16_t basev, uint16_t maxs, uint16_t * minv, uint16_t * sides, uint16_t index) {
	uint8_t in_menu = 1, j;
	render_smenu(0xFF, title, *minv, *sides, index);
	while (in_menu) {
		if ((j = input_get())) {
			switch (j) {
				case INPUT_UP:
					if (index > 0) {
						index--;
						render_smenu(0x60 >> index, title, *minv, *sides, index);
					} else {
						index = 4;
						render_smenu(0x44, title, *minv, *sides, index);
					}
					break;
				case INPUT_DN:
					if (index < 4) {
						index++;
						render_smenu(0xC0 >> index, title, *minv, *sides, index);
					} else {
						index = 0;
						render_smenu(0x44, title, *minv, *sides, index);
					}
					break;
				case INPUT_LT:
					switch (index) {
						case 0:
							if ((*minv) > basev) {
								(*minv)--;
								render_smenu(0x70, title, *minv, *sides, index);
							}
							break;
						case 1: case 2:
							if ((*sides) > 1) {
								(*sides)--;
								render_smenu(0x70, title, *minv, *sides, index);
							}
							break;
						default:
							index = 4;
							render_smenu(0x7C, title, *minv, *sides, index);
							break;
					}
					break;
				case INPUT_RT:
					switch (index) {
						case 0:
							if ((*minv) < (basev + maxs - (*sides))) {
								(*minv)++;
								render_smenu(0x70, title, *minv, *sides, index);
							}
							break;
						case 1: case 2:
							if ((*sides) < (basev + maxs - (*minv))) {
								(*sides)++;
								render_smenu(0x70, title, *minv, *sides, index);
							}
							break;
						default:
							index = 3;
							render_smenu(0x7C, title, *minv, *sides, index);
							break;
					}
					break;
				case INPUT_CTR:
					if (index < 3) {
						index++;
						render_smenu(0xC0 >> index, title, *minv, *sides, index);
					} else {
						tft.fillScreen(0);
						in_menu = 0;
					}
					break;
				default:
					continue;
			}
			delay(50);
			while (input_get());
			delay(50);
		}
	}
	return index;
}

static void write_mem_header() {
	uint16_t i;
	dx_seek(mem, 0, 0, 0);
	fs.buf[ 0] = currCount >> 8;
	fs.buf[ 1] = currCount;
	fs.buf[ 2] = orderType >> 8;
	fs.buf[ 3] = orderType;
	fs.buf[ 4] = '~';
	fs.buf[ 5] = '~';
	fs.buf[ 6] = 'S';
	fs.buf[ 7] = 'A';
	fs.buf[ 8] = 'V';
	fs.buf[ 9] = 'E';
	fs.buf[10] = '~';
	fs.buf[11] = '~';
	fs.buf[12] = currLayout >> 8;
	fs.buf[13] = currLayout;
	fs.buf[14] = random(256);
	fs.buf[15] = random(256);
	for (i = 16; i < 512; i++) fs.buf[i] = -1;
	fs.write();
}

static uint8_t load_preset_menu() {
	uint16_t roll, ptr, i, j;
	uint8_t buf[16];
	roll = rmenu((char *)main_menu_items[1], rollIndex, rollCount, 0);
	if (roll >= rollCount) return 0;
	dx_seek(rollData, roll, 0, 0); fs.read();
	currCount = fs.buf[0]; currCount <<= 8; currCount |= fs.buf[1];
	orderType = fs.buf[2]; orderType <<= 8; orderType |= fs.buf[3];
	write_mem_header();
	for (i = 0; i < currCount; i++) {
		ptr = dx_seek(rollData, roll, 1, i << 4); fs.read();
		for (j = 0; j < 16; j++) buf[j] = fs.buf[ptr++];
		dx_seek(mem, 0, i + 1, 0);
		for (j = 0; j < 16; j++) fs.buf[j] = buf[j];
		for (j = 16; j < 512; j++) fs.buf[j] = -1;
		fs.write();
	}
	order_dice(1);
	roll_dice();
	render_dice();
	return 1;
}

static uint8_t replace_menu() {
	uint16_t dice, type, basev, defs, maxs, minv, sides, i;
	dice = rmenu((char *)main_menu_items[2], diceIndex, diceCount, 0);
	if (dice >= diceCount) return 0;
	dx_seek(diceData, dice, 1, 0); fs.read();
	type  = fs.buf[0]; type  <<= 8; type  |= fs.buf[1];
	basev = fs.buf[2]; basev <<= 8; basev |= fs.buf[3];
	defs  = fs.buf[4]; defs  <<= 8; defs  |= fs.buf[5];
	maxs  = fs.buf[6]; maxs  <<= 8; maxs  |= fs.buf[7];
	switch (type) {
		case 0:
			minv = 0;
			sides = 0;
			break;
		case 1:
			minv = basev ? basev : 1;
			sides = defs;
			i = smenu((char *)main_menu_items[2], basev, maxs, &minv, &sides, 0);
			if (i >= 4) return 0;
			break;
		case 2:
			minv = 1;
			sides = defs;
			i = smenu((char *)main_menu_items[2], 0, maxs, &minv, &sides, 0);
			if (i >= 4) return 0;
			break;
	}
	dx_seek(mem, 0, 1, 0);
	fs.buf[ 0] = dice  >> 8; fs.buf[ 1] = dice ;
	fs.buf[ 2] = type  >> 8; fs.buf[ 3] = type ;
	fs.buf[ 4] = 0         ; fs.buf[ 5] = 0    ;
	fs.buf[ 6] = minv  >> 8; fs.buf[ 7] = minv ;
	fs.buf[ 8] = sides >> 8; fs.buf[ 9] = sides;
	fs.buf[10] = basev >> 8; fs.buf[11] = basev;
	fs.buf[12] = defs  >> 8; fs.buf[13] = defs ;
	fs.buf[14] = maxs  >> 8; fs.buf[15] = maxs ;
	for (i = 16; i < 512; i++) fs.buf[i] = -1;
	fs.write();
	currCount = 1;
	orderType = 0;
	write_mem_header();
	order_dice(1);
	roll_dice();
	render_dice();
	return 1;
}

static uint8_t add_menu() {
	uint16_t dice, type, basev, defs, maxs, minv, sides, i;
	if (currCount >= MAX_DICE) {
		error_message(25, (char *)error_messages[0], 0, 0, 0xFFE0);
		return 0;
	}
	dice = rmenu((char *)main_menu_items[3], diceIndex, diceCount, 0);
	if (dice >= diceCount) return 0;
	dx_seek(diceData, dice, 1, 0); fs.read();
	type  = fs.buf[0]; type  <<= 8; type  |= fs.buf[1];
	basev = fs.buf[2]; basev <<= 8; basev |= fs.buf[3];
	defs  = fs.buf[4]; defs  <<= 8; defs  |= fs.buf[5];
	maxs  = fs.buf[6]; maxs  <<= 8; maxs  |= fs.buf[7];
	switch (type) {
		case 0:
			minv = 0;
			sides = 0;
			break;
		case 1:
			minv = basev ? basev : 1;
			sides = defs;
			i = smenu((char *)main_menu_items[3], basev, maxs, &minv, &sides, 0);
			if (i >= 4) return 0;
			break;
		case 2:
			minv = 1;
			sides = defs;
			i = smenu((char *)main_menu_items[3], 0, maxs, &minv, &sides, 0);
			if (i >= 4) return 0;
			break;
	}
	dx_seek(mem, 0, currCount + 1, 0);
	fs.buf[ 0] = dice  >> 8; fs.buf[ 1] = dice ;
	fs.buf[ 2] = type  >> 8; fs.buf[ 3] = type ;
	fs.buf[ 4] = 0         ; fs.buf[ 5] = 0    ;
	fs.buf[ 6] = minv  >> 8; fs.buf[ 7] = minv ;
	fs.buf[ 8] = sides >> 8; fs.buf[ 9] = sides;
	fs.buf[10] = basev >> 8; fs.buf[11] = basev;
	fs.buf[12] = defs  >> 8; fs.buf[13] = defs ;
	fs.buf[14] = maxs  >> 8; fs.buf[15] = maxs ;
	for (i = 16; i < 512; i++) fs.buf[i] = -1;
	fs.write();
	currCount++;
	write_mem_header();
	order_dice(1);
	roll_dice();
	render_dice();
	return 1;
}

static uint8_t edit_menu() {
	uint16_t index, dice, type, value, basev, defs, maxs, minv, sides, i;
	index = dmenu((char *)main_menu_items[4], 0);
	if (index >= currCount) return 0;
	dx_seek(mem, 0, index + 1, 0); fs.read();
	dice  = fs.buf[ 0]; dice  <<= 8; dice  |= fs.buf[ 1];
	type  = fs.buf[ 2]; type  <<= 8; type  |= fs.buf[ 3];
	value = fs.buf[ 4]; value <<= 8; value |= fs.buf[ 5];
	minv  = fs.buf[ 6]; minv  <<= 8; minv  |= fs.buf[ 7];
	sides = fs.buf[ 8]; sides <<= 8; sides |= fs.buf[ 9];
	basev = fs.buf[10]; basev <<= 8; basev |= fs.buf[11];
	defs  = fs.buf[12]; defs  <<= 8; defs  |= fs.buf[13];
	maxs  = fs.buf[14]; maxs  <<= 8; maxs  |= fs.buf[15];
	switch (type) {
		case 0:
			error_message(25, (char *)error_messages[1], 0, 0, 0xFFE0);
			return 0;
		case 1:
			i = smenu((char *)main_menu_items[4], basev, maxs, &minv, &sides, 0);
			if (i >= 4) return 0;
			break;
		case 2:
			i = smenu((char *)main_menu_items[4], 0, maxs, &minv, &sides, 0);
			if (i >= 4) return 0;
			break;
	}
	dx_seek(mem, 0, index + 1, 0);
	fs.buf[ 0] = dice  >> 8; fs.buf[ 1] = dice ;
	fs.buf[ 2] = type  >> 8; fs.buf[ 3] = type ;
	fs.buf[ 4] = value >> 8; fs.buf[ 5] = value;
	fs.buf[ 6] = minv  >> 8; fs.buf[ 7] = minv ;
	fs.buf[ 8] = sides >> 8; fs.buf[ 9] = sides;
	fs.buf[10] = basev >> 8; fs.buf[11] = basev;
	fs.buf[12] = defs  >> 8; fs.buf[13] = defs ;
	fs.buf[14] = maxs  >> 8; fs.buf[15] = maxs ;
	for (i = 16; i < 512; i++) fs.buf[i] = -1;
	fs.write();
	order_dice(1);
	roll_dice();
	render_dice();
	return 1;
}

static uint8_t remove_menu() {
	uint16_t index, i;
	uint8_t buf[16];
	if (currCount < 2) {
		error_message(10, (char *)error_messages[2], 0, 0, 0xFFE0);
		return 0;
	}
	index = dmenu((char *)main_menu_items[5], 0);
	if (index >= currCount) return 0;
	currCount--;
	write_mem_header();
	while (index < currCount) {
		dx_seek(mem, 0, index + 2, 0); fs.read();
		for (i = 0; i < 16; i++) buf[i] = fs.buf[i];
		dx_seek(mem, 0, index + 1, 0);
		for (i = 0; i < 16; i++) fs.buf[i] = buf[i];
		for (i = 16; i < 512; i++) fs.buf[i] = -1;
		fs.write();
		index++;
	}
	order_dice(1);
	roll_dice();
	render_dice();
	return 1;
}

static uint8_t set_order_menu() {
	uint16_t order;
	order = amenu((char *)main_menu_items[6], (char **)set_order_menu_items, SET_ORDER_MENU_ITEMS, orderType);
	if (order >= SET_ORDER_MENU_ITEMS) return 0;
	orderType = order;
	write_mem_header();
	order_dice(1);
	render_dice();
	return 1;
}

static uint8_t set_layout_menu() {
	uint16_t layout;
	layout = rmenu((char *)main_menu_items[7], layoutIndex, layoutCount, currLayout);
	if (layout >= layoutCount) return 0;
	currLayout = layout;
	write_mem_header();
	render_dice();
	return 1;
}

static uint8_t main_menu() {
	uint16_t index = 0;
	for (;;) {
		index = amenu((char *)main_menu_title, (char **)main_menu_items, MAIN_MENU_ITEMS, index);
		switch (index) {
			case 0: order_dice(0); roll_dice(); render_dice(); return 1;
			case 1: if (load_preset_menu()) return 1; break;
			case 2: if (replace_menu()) return 1; break;
			case 3: if (add_menu()) return 1; break;
			case 4: if (edit_menu()) return 1; break;
			case 5: if (remove_menu()) return 1; break;
			case 6: if (set_order_menu()) return 1; break;
			case 7: if (set_layout_menu()) return 1; break;
			case 8: return 0;
			case 9: render_dice(); return 1;
		}
	}
}



/* ---- DX MAIN ---- */

uint8_t dx_setup(void) {
	if (load_4dx()) {
		tft.fillScreen(0);
		order_dice(1);
		roll_dice();
		render_dice();
		return 1;
	} else {
		error_message(16, (char *)dx_not_found, 0, 0, -1);
		return 0;
	}
}

uint8_t dx_loop(void) {
	uint8_t j = input_get();
	if (j == INPUT_CTR) {
		tft.fillScreen(0);
		delay(50);
		while (input_get());
		delay(50);
		if (!main_menu()) {
			fs.close();
			return 0;
		}
	} else if (j) {
		order_dice(0);
		roll_dice();
		if (j != INPUT_VS) {
			delay(50);
			while (input_get()) {
				render_dice();
				order_dice(0);
				roll_dice();
			}
			delay(50);
		}
		render_dice();
	}
	return 1;
}
