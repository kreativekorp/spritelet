/**********************************************************
 * PicoFS FAT Library for Arduino by Rebecca Bettencourt. *
 * Based on PetitFS by ChaN. MIT license.                 *
 **********************************************************/

#ifndef __PICOFS_H__
#define __PICOFS_H__

#include <Arduino.h>

#define FS_FAT12 1
#define FS_FAT16 2
#define FS_FAT32 3

#define AM_RDO  0x01
#define AM_HID  0x02
#define AM_SYS  0x04
#define AM_VOL  0x08
#define AM_LFN  0x0F
#define AM_DIR  0x10
#define AM_ARC  0x20
#define AM_MASK 0x3F

#define FR_OK            0
#define FR_DISK_ERR      1
#define FR_NOT_READY     2
#define FR_NO_FILE       3
#define FR_NOT_OPENED    4
#define FR_NOT_ENABLED   5
#define FR_NO_FILESYSTEM 6
#define FR_FRAGMENTED    7
#define FR_EOF           8

#define MAX_FRAGMENTS 16

class FATFS {
	public:
		uint8_t buf[512];
		uint32_t file_size;
		uint32_t frag_offsets[MAX_FRAGMENTS];
		uint32_t frag_lengths[MAX_FRAGMENTS];
		uint8_t num_fragments;

		void init(boolean startSPI);
		uint8_t mount(void);
		uint8_t open(char * path);
		uint8_t seek(uint32_t sector);
		uint8_t read(void);
		uint8_t write(void);
		void close(void);

	private:
		uint8_t fs_type;
		uint8_t cluster_size;
		uint16_t num_root_files;
		uint32_t num_clusters;
		uint32_t fat_sector;
		uint32_t root_sector;
		uint32_t data_sector;

		uint32_t last_fat_sector;
		uint32_t start_cluster;
		uint32_t curr_cluster;
		uint32_t curr_sector;
		uint8_t curr_fragment;

		uint8_t check_fs(uint32_t sector);
		uint32_t get_fat(uint32_t cluster);
		uint8_t get_fragments(void);
		char * next_name(char * path, char * name);
		uint8_t name_cmp(char * n1, char * n2);
		uint8_t find_name(char * name);
};

#endif
