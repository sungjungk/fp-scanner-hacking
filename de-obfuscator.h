/* 
 * Fingerprint de-obfuscatior for libfprint(uru4000)
 * Copyright (C) 2019 Seong-Joong Kim <sungjungk@gmail.com>
 *
 * It is based on libfprint/uru4000 driver.
 */

#ifndef _DEOBFUSCATOR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <errno.h>

#define END_OF_SEED_DICTIONARY    5
#define IMAGE_HEIGHT		          290
#define IMAGE_WIDTH		            384


enum {
	BLOCKF_CHANGE_KEY	    = 0x80,
	BLOCKF_NO_KEY_UPDATE	= 0x04,
	BLOCKF_ENCRYPTED		  = 0x02,
	BLOCKF_NOT_PRESENT	  = 0x01,
};

typedef struct raw_img raw_img_t;
struct raw_img {
	uint8_t		unknown_00[4];
	uint16_t	row_pos;
	uint8_t		key_number;
	uint8_t		unknown_07[9];
	struct {
		uint8_t	flags;
		uint8_t	row_pos;
	} block_info[15];
	uint8_t		unknown_2E[18];
	uint8_t		data[IMAGE_HEIGHT][IMAGE_WIDTH];
};

typedef struct fp_img fp_img_t;
struct fp_img {
	int width;
	int height;
	size_t length;
	uint16_t flags;
	int *dummy[2];
	unsigned char data[0];
};


/* import a encoded fingerprint data */
static inline bool file_import(const char *path, uint8_t**buf, int *file_len)
{
  FILE *fp = NULL;
  int i = 0, rc = 0;

  fp = fopen(path, "rb");
  if (!fp) {
    g_warning("Fail to fopen()");
    return false;
  }

  fseek(fp, 0, SEEK_END);
  *file_len = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  *buf = g_malloc(*file_len);
  for (i = 0; i < *file_len; i++) {
    rc = getc(fp);
    *(*buf+i) = rc;
  }
  
  g_debug("Imported file length: %d", *file_len);

  fclose(fp);

  return true;
}


/* 
 * utils for decoding fingerprint data 
 */

static inline uint32_t update_key(uint32_t key)
{
	uint32_t bit = key & 0x9248144d;
	
	bit ^= bit << 16;
	bit ^= bit << 8;
	bit ^= bit << 4;
	bit ^= bit << 2;
	bit ^= bit << 1;

	return (bit & 0x80000000) | (key >> 1);
}

static inline uint32_t do_decode(uint8_t *data, int num_bytes, uint32_t key)
{
	uint8_t xorbyte;
	int i;

	for (i = 0; i < num_bytes - 1; i++) {
		xorbyte  = ((key >>  4) & 1) << 0;
		xorbyte |= ((key >>  8) & 1) << 1;
		xorbyte |= ((key >> 11) & 1) << 2;
		xorbyte |= ((key >> 14) & 1) << 3;
		xorbyte |= ((key >> 18) & 1) << 4;
		xorbyte |= ((key >> 21) & 1) << 5;
		xorbyte |= ((key >> 24) & 1) << 6;
		xorbyte |= ((key >> 29) & 1) << 7;
		key = update_key(key);

		data[i] = data[i+1] ^ xorbyte;
	}

	data[i] = 0;

	return update_key(key);
}

/* save decoded fingerprint data to PGM file */
static inline int image_save(fp_img_t *img, char *path)
{
	FILE *fd = fopen(path, "w");
	size_t write_size = img->width * img->height;
	int r;

	if (!fd) {
		g_warning("could not open '%s' for writing: %d", path, errno);
		return -errno;
	}

	r = fprintf(fd, "P5 %d %d 255\n", img->width, img->height);
	if (r < 0) {
		g_warning("pgm header write failed, error %d", r);
		return r;
	}

	r = fwrite(img->data, 1, write_size, fd);
	if (r < write_size) {
		g_warning("short write (%d)", r);
		return -EIO;
	}

	fclose(fd);

	g_debug("written to '%s'", path);

	return 0;
}

#endif

