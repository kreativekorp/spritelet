/**********************************************************
 * PicoFS FAT Library for Arduino by Rebecca Bettencourt. *
 * Based on PetitFS by ChaN. MIT license.                 *
 **********************************************************/

#include <Arduino.h>
#include "picofs_diskio.h"
#include "picofs_pins.h"
#include "picofs.h"
#include "spi.h"

#define UINT16(x) ((uint16_t)( *( (uint16_t *)(uint8_t *)(&(x)) ) ))
#define UINT32(x) ((uint32_t)( *( (uint32_t *)(uint8_t *)(&(x)) ) ))

void FATFS::init(boolean startSPI) {
  SD_CS_DDR  |= SD_CS_MASK;
  SD_CS_PORT |= SD_CS_MASK;

  if (startSPI) {
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setDataMode(SPI_MODE0);
  }
}

uint8_t FATFS::check_fs(uint32_t sector) {
  if (disk_read(buf, sector)) return FR_DISK_ERR;
  if (UINT16(buf[510]) != 0xAA55) return FR_NO_FILESYSTEM;
  fs_type = (UINT16(buf[54]) == 0x4146 || UINT16(buf[82]) == 0x4146);
  return FR_OK;
}

uint8_t FATFS::mount(void) {
  uint8_t res;

  // start of partition
  if (disk_initialize()) return FR_NOT_READY;
  if ((res = check_fs(0))) return res;
  if (fs_type) curr_sector = 0;
  else if (!buf[450]) return FR_NO_FILESYSTEM;
  else {
    curr_sector = UINT32(buf[454]);
    if ((res = check_fs(curr_sector))) return res;
    if (!fs_type) return FR_NO_FILESYSTEM;
  }

  // FAT size * number of FATs
  file_size = UINT16(buf[22]);
  if (!file_size) file_size = UINT32(buf[36]);
  file_size *= buf[16];

  cluster_size = buf[13];
  fat_sector = curr_sector + UINT16(buf[14]);
  num_root_files = UINT16(buf[17]);

  num_clusters = UINT16(buf[19]);
  if (!num_clusters) num_clusters = UINT32(buf[32]);
  num_clusters -= UINT16(buf[14]);      // reserved sectors
  num_clusters -= file_size;            // FAT
  num_clusters -= num_root_files >> 4;  // root directory
  num_clusters /= cluster_size;
  num_clusters += 2;

  if (num_clusters < 0xFF7) fs_type = FS_FAT12;
  else if (num_clusters < 0xFFF7) fs_type = FS_FAT16;
  else fs_type = FS_FAT32;

  root_sector = num_root_files ? (fat_sector + file_size) : UINT32(buf[44]);
  data_sector = fat_sector + file_size + (num_root_files >> 4);

  close();
  return FR_OK;
}

uint32_t FATFS::get_fat(uint32_t cluster) {
  if (cluster < 2 || cluster >= num_clusters) return 0;
  switch (fs_type) {
    case FS_FAT12:
      {
        uint16_t wc, bc, ofs;
        uint32_t sector;
        bc = cluster + (cluster >> 1);
        ofs = bc & 0x1FF; bc >>= 9;
        if (ofs == 511) {
          sector = fat_sector + bc + 1;
          if ((sector != last_fat_sector) && disk_read(buf, last_fat_sector = sector)) return 0;
          wc = buf[0]; wc <<= 8;
          last_fat_sector--;
          if (disk_read(buf, last_fat_sector)) return 0;
          wc |= buf[511];
        } else {
          sector = fat_sector + bc;
          if ((sector != last_fat_sector) && disk_read(buf, last_fat_sector = sector)) return 0;
          wc = UINT16(buf[ofs]);
        }
        cluster = (cluster & 1) ? (wc >> 4) : (wc & 0xFFF);
      }
      break;
    case FS_FAT16:
      {
        uint32_t sector = fat_sector + (cluster >> 8);
        if ((sector != last_fat_sector) && disk_read(buf, last_fat_sector = sector)) return 0;
        cluster = UINT16(buf[(cluster & 0xFF) << 1]);
      }
      break;
    case FS_FAT32:
      {
        uint32_t sector = fat_sector + (cluster >> 7);
        if ((sector != last_fat_sector) && disk_read(buf, last_fat_sector = sector)) return 0;
        cluster = UINT32(buf[(cluster & 0x7F) << 2]) & 0x0FFFFFFF;
      }
      break;
  }
  if (cluster < 2 || cluster >= num_clusters) return 0;
  return cluster;
}

uint8_t FATFS::get_fragments(void) {
  if (start_cluster == 1) {
    // root directory (FAT12/FAT16)
    frag_offsets[0] = root_sector;
    frag_lengths[0] = data_sector - root_sector;
    num_fragments = 1;
  } else {
    // file / subdirectory
    uint32_t next_cluster;
    uint8_t ptr;
    last_fat_sector = 0;
    curr_cluster = start_cluster;
    frag_offsets[0] = data_sector + ((curr_cluster - 2) * cluster_size);
    frag_lengths[0] = cluster_size;
    ptr = 0;
    for (;;) {
      next_cluster = get_fat(curr_cluster);
      if (!next_cluster) {
        num_fragments = ptr + 1;
        break;
      } else if (next_cluster == curr_cluster + 1) {
        curr_cluster = next_cluster;
        frag_lengths[ptr] += cluster_size;
      } else {
        ptr++;
        if (ptr < MAX_FRAGMENTS) {
          curr_cluster = next_cluster;
          frag_offsets[ptr] = data_sector + ((curr_cluster - 2) * cluster_size);
          frag_lengths[ptr] = cluster_size;
        } else {
          num_fragments = -1;
          return FR_FRAGMENTED;
        }
      }
    }
  }
  curr_cluster = 0;
  curr_sector = 0;
  curr_fragment = 0;
  return FR_OK;
}

char * FATFS::next_name(char * path, char * name) {
  uint8_t ptr = 0;
  while ((*path) == '/' || (*path) == '\\') path++;
  if (!(*path)) return 0;
  while ((*path) && (*path) != '/' && (*path) != '\\' && (*path) != '.') {
    if (ptr < 8) name[ptr++] = *path;
    path++;
  }
  while (ptr < 8) name[ptr++] = ' ';
  while ((*path) == '.') path++;
  while ((*path) && (*path) != '/' && (*path) != '\\') {
    if (ptr < 11) name[ptr++] = *path;
    path++;
  }
  while (ptr < 11) name[ptr++] = ' ';
  name[ptr] = 0;
  return path;
}

uint8_t FATFS::name_cmp(char * n1, char * n2) {
  uint8_t ptr, cmp;
  for (ptr = 0; ptr < 11; ptr++) {
    cmp = n1[ptr] - n2[ptr];
    if (cmp) return cmp;
  }
  return 0;
}

uint8_t FATFS::find_name(char * name) {
  uint16_t ptr;
  while (!read()) {
    for (ptr = 0; ptr < 512; ptr += 32) {
      if (!name_cmp((char *)&buf[ptr], name)) {
        file_size = UINT32(buf[ptr + 28]);
        if (fs_type == FS_FAT32) {
          start_cluster = UINT16(buf[ptr + 20]);
          start_cluster <<= 16;
          start_cluster |= UINT16(buf[ptr + 26]);
        } else {
          start_cluster = UINT16(buf[ptr + 26]);
        }
        return FR_OK;
      }
    }
  }
  return FR_NO_FILE;
}

uint8_t FATFS::open(char * path) {
  char name[12];
  file_size = 0;
  start_cluster = (num_root_files ? 1 : root_sector);
  if (get_fragments()) { close(); return FR_FRAGMENTED; }
  while ((path = next_name(path, name))) {
    if (find_name(name)) { close(); return FR_NO_FILE; }
    if (get_fragments()) { close(); return FR_FRAGMENTED; }
  }
  return FR_OK;
}

uint8_t FATFS::seek(uint32_t sector) {
  if (!start_cluster) return FR_NOT_OPENED;
  for (curr_fragment = 0; curr_fragment < num_fragments; curr_fragment++) {
    if (sector < frag_lengths[curr_fragment]) {
      curr_sector = sector;
      return FR_OK;
    } else {
      sector -= frag_lengths[curr_fragment];
    }
  }
  return FR_EOF;
}

uint8_t FATFS::read(void) {
  uint32_t sector;
  if (!start_cluster) return FR_NOT_OPENED;
  if (curr_fragment >= num_fragments) return FR_EOF;
  sector = frag_offsets[curr_fragment] + curr_sector;
  if (disk_read(buf, sector)) return FR_DISK_ERR;
  curr_sector++;
  if (curr_sector >= frag_lengths[curr_fragment]) {
    curr_sector = 0;
    curr_fragment++;
  }
  return FR_OK;
}

uint8_t FATFS::write(void) {
  uint32_t sector;
  if (!start_cluster) return FR_NOT_OPENED;
  if (curr_fragment >= num_fragments) return FR_EOF;
  sector = frag_offsets[curr_fragment] + curr_sector;
  if (disk_write(buf, sector)) return FR_DISK_ERR;
  curr_sector++;
  if (curr_sector >= frag_lengths[curr_fragment]) {
    curr_sector = 0;
    curr_fragment++;
  }
  return FR_OK;
}

void FATFS::close(void) {
  file_size = 0;
  start_cluster = 0;
  curr_cluster = 0;
  curr_sector = 0;
  num_fragments = 0;
  curr_fragment = 0;
}
