/* 
 * Fingerprint de-obfuscatior for libfprint(uru4000)
 * Copyright (C) 2019 Seong-Joong Kim <sungjungk@gmail.com>
 *
 * It is based on libfprint/uru4000 driver.
 */

#include <stdio.h>
#include <glib.h>
#include "de-obfuscator.h"


raw_img_t *
decode_image(uint8_t *buf, uint32_t key)
{
  int block_id = 0,  row = 0, pos = 0, row_pos = 0, flags = 0;

  raw_img_t *img = (raw_img_t*)buf;

  /* do decode */
  while (block_id < G_N_ELEMENTS(img->block_info) && pos < img->row_pos) {
    /* setup flags & row_pos */
    flags = img->block_info[block_id].flags;
    row_pos = img->block_info[block_id].row_pos;

    if (flags == BLOCKF_ENCRYPTED) {
      key = do_decode(&img->data[pos][0], row_pos * IMAGE_WIDTH, key);
    } else {
      for (row = 0; row < row_pos * IMAGE_WIDTH; row++)
        key = update_key(key);
    }

    if (!(flags & BLOCKF_NOT_PRESENT))
      pos += row_pos;
    block_id++;
  }

  return img;
}

static fp_img_t *fp_img_init(void)
{
  int length = IMAGE_HEIGHT * IMAGE_WIDTH;

  fp_img_t *fp_img = g_malloc(sizeof(*fp_img) + length);
  fp_img->width = IMAGE_WIDTH;
  fp_img->height = IMAGE_HEIGHT;
  fp_img->length = length;

	return fp_img;
}

fp_img_t *
restore_image(raw_img_t *raw_img)
{
  int block_id = 0,  row = 0, pos = 0, row_pos = 0, flags = 0;
  fp_img_t *fp_img;

  /* make a clear fingerprint image */
  fp_img = fp_img_init();

  block_id = pos = row = 0;
  while (block_id < G_N_ELEMENTS(raw_img->block_info) &&
         row < raw_img->row_pos) {
    flags = raw_img->block_info[block_id].flags;
    row_pos = raw_img->block_info[block_id].row_pos;

    memcpy(&fp_img->data[pos], &raw_img->data[row][0], row_pos * IMAGE_WIDTH);
    if (!(flags & BLOCKF_NOT_PRESENT))
      row += row_pos;
    pos += row_pos * IMAGE_WIDTH;

    block_id++;
  }

  fp_img->width = IMAGE_WIDTH;
  fp_img->height = IMAGE_HEIGHT;

  return fp_img;
}

static uint32_t 
key_gen(char *argv[])
{
  gint64 key_byte[4], seed_byte[4];
  uint32_t key_val = 0, seed_val = 0;

  seed_byte[0] = g_ascii_strtoull(argv[1], NULL, 16);
  seed_byte[1] = g_ascii_strtoull(argv[2], NULL, 16);
  seed_byte[2] = g_ascii_strtoull(argv[3], NULL, 16);
  seed_byte[3] = g_ascii_strtoull(argv[4], NULL, 16);

  seed_val = seed_byte[0];
  seed_val |= seed_byte[1] << 8;
  seed_val |= seed_byte[2] << 16;
  seed_val |= seed_byte[3] << 24;

  key_byte[0] = g_ascii_strtoull(argv[5], NULL, 16);
  key_byte[1] = g_ascii_strtoull(argv[6], NULL, 16);
  key_byte[2] = g_ascii_strtoull(argv[7], NULL, 16);
  key_byte[3] = g_ascii_strtoull(argv[8], NULL, 16);

  key_val = key_byte[0];
  key_val |= key_byte[1] << 8;
  key_val |= key_byte[2] << 16;
  key_val |= key_byte[3] << 24;
  key_val ^= seed_val;

  return key_val;
}

int 
main(int argc, char *argv[])
{
  uint32_t key = 0;
  uint8_t *buf = NULL;
  int file_len = 0, i = 0;
  bool status = false;

  raw_img_t *raw_img = NULL;
  fp_img_t *fp_img = NULL;
  gchar *file_name = NULL;


  g_setenv ("G_MESSAGES_DEBUG", "all", TRUE);

  if (9 != argc) {
    g_print ("Usage: %s seed[0-3] key_byte[0-3]\n", argv[0]);
    return EXIT_FAILURE;
  }

  status = file_import("encoded_finger.bin", (uint8_t**)&buf, &file_len);
  if (false == status) {
    g_warning("fail to file_import");
    return EXIT_FAILURE;
  }

  key = key_gen(argv);
  raw_img = decode_image(buf, key);
  if (!raw_img) {
    g_warning("fail to decode_image");
    return EXIT_FAILURE;
  }

  fp_img = restore_image(raw_img);
  if (!fp_img) {
    g_warning("fail to restore_image");
    return EXIT_FAILURE;
  }

  file_name = g_strdup_printf("%d_encoded_finger.pgm", i);
  image_save(fp_img, file_name);

  g_free(file_name);

  g_free(buf);

  return 0;
}

/* end of fp_decoder.c */
