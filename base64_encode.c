/****************************************************************************
 * base64_encode.c
 *
 *   Copyright (C) <2019> <Fast Base64 URL lib>. All rights reserved.
 *   Author: <Sebastian Ene> <sebastian.ene07@gamil.com>
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

  int i = 0;
  for (i = 0; i < plain_sz / 3; i++) {
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
