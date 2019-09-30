/****************************************************************************
 * base64_encode.c
 *
 *   Copyright (C) <2019> <Fast Base64 URL lib>. All rights reserved.
 *   Author: Sebastian Ene <sebastian.ene07@gamil.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "base64_encode.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uint8_t get_base64_alphabet_index(uint8_t c)
{
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  } else if (c >= 'a' && c <= 'z') {
    return c - 'a' + ('Z' - 'A' + 1);
  } else if (c >= '0' && c <= '9') {
    return c - '0' + ('z' - 'a' + 1) + ('Z' - 'A' + 1);
  } else if (c == '-') {
    return 63;
  } else if (c == '_') {
    return 64;
  } else {
    return INVALID_BASE64_URL_CHAR;
  }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int base64_encode(const char *text, char **encoded_data, size_t *encoded_len)
{
  const char *base_64_alphabet = BASE_64_URL_ALPHABET;

  if (text == NULL ||
      encoded_data == NULL ||
      encoded_len == NULL) {
    return -EINVAL;
  }

  /* Assuming the code runs on little endian */
  size_t plain_sz = strlen(text);
  *encoded_len = (plain_sz / 3 + (plain_sz % 3 ? 1 : 0)) * 4;
  *encoded_data = calloc(1, *encoded_len);
  if (*encoded_data == NULL) {
    return -ENOMEM;
  }

  int i = 0;
  for (i ; i < plain_sz / 3; i++) {
    uint32_t acc = (*((uint32_t *)(text + i * 3)) & 0xFFFFFF);

    /* Apply mask to swap byte order in memory */
    acc = ((acc & 0xFF) << 16) | (acc & 0xFF00) | ((acc & 0xFF0000) >> 16);

    uint32_t *encode = ((uint32_t *)*encoded_data) + i;
    *encode = (base_64_alphabet[acc & (0x3F)] << 24) |
              (base_64_alphabet[(acc & (0x3F << 6)) >> 6] << 16) |
              (base_64_alphabet[(acc & (0x3F << 12)) >> 12] << 8) |
              (base_64_alphabet[(acc & (0x3F << 18)) >> 18]);
  }

  if (plain_sz % 3) {
    /* Take the remaining bytes */
    uint32_t acc = 0, index = 0;
    for (int j = i * 3; j < plain_sz; j++) {
      acc |= (text[j] << (index * 8));
      index++;
    }

    /* Apply mask to swap byte order in memory */
    acc = ((acc & 0xFF) << 16) | (acc & 0xFF00) | ((acc & 0xFF0000) >> 16);
    uint32_t *encode = ((uint32_t *)*encoded_data) + i;

    /* Add remaining bytes plain_sz % 3 */
    for (int i = 0; i <= plain_sz % 3; i++) {
       uint8_t shift_mask = 18 - i * 6;
       uint8_t shift_byte_final_pos = 8 * i;

       *encode |= (base_64_alphabet[(acc & (0x3F << shift_mask)) >> shift_mask]
          << shift_byte_final_pos);
    }

    /* Update the final length */
    *encoded_len = strlen(*encoded_data);
  }

  return OK;
}

int base64_decode(char **decoded_text, size_t *decoded_len,
  const char *encoded_data)
{
  size_t encoded_len = strlen(encoded_data);
  int i = 0, plain_index = 0;

  char *plain_text = calloc(1, encoded_len);
  if (plain_text == NULL) {
    return -ENOMEM;
  }

  for (i ; i < encoded_len / 4; i++) {

    uint32_t *plain = ((uint32_t *)(plain_text + plain_index));

    *plain = (get_base64_alphabet_index(encoded_data[3 + i * 4]) & 0x3F) |
             ((get_base64_alphabet_index(encoded_data[2 + i *4 ]) & 0x3F) << 6) |
             ((get_base64_alphabet_index(encoded_data[1 + i * 4]) & 0x3F) << 12) |
             ((get_base64_alphabet_index(encoded_data[i * 4]) & 0x3F) << 18);

    *plain = ((*plain & 0xFF) << 16) |
             (*plain & 0xFF00) |
             ((*plain & 0xFF0000) >> 16);

    plain_index += 3;
  }

  if (encoded_len % 4) {
    uint32_t *plain = ((uint32_t *)(plain_text + plain_index));

    int j = i * 4;
    for (j; j < encoded_len; j++) {
      int shift_mask = j - i * 4;

      *plain |= (get_base64_alphabet_index(encoded_data[j]) & 0x3F)
        << (18 - shift_mask * 6);
    }

    *plain = ((*plain & 0xFF) << 16) |
             (*plain & 0xFF00) |
             ((*plain & 0xFF0000) >> 16);
  }

  *decoded_text = plain_text;
  *decoded_len  = strlen(plain_text);

  return OK;
}
