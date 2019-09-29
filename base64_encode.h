/****************************************************************************
 * base64_encode.h
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

#ifndef __BASE64_ENCODE_H
#define __BASE64_ENCODE_H

/****************************************************************************
 * Public Macro Definitions
 ****************************************************************************/

#define OK                      (0)
#define LENGTH(x)               (sizeof(x) / sizeof((x)[0]))
#define BASE_64_ALPHABET_LEN    (64)
#define BASE_64_GROUP_LEN       (4)

#define BASE_64_URL_ALPHABET     \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Test structure content */

typedef struct {
  const char *encoded_data;
  const char *plain_data;
} base64_encode_test_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: base64_encode
 *
 * Description:
 *   This method encodes an input null terminated string into a base64_url
 *   format and saves the result in a specified location. This method
 *   allocates memory for the new encoded string.
 *
 * Input Parameters:
 *   text            - [in] NULL terminated plain text string
 *   encoded_data    - [out] location where we store the pointer to the encoded
 *                    data buffer.
 *   encoded_len     - [out] location where we store the size of the
 *                     encoded_data
 *
 * Returned Value:
 *   OK in case of success otherwise a negative error code.
 *
 * Assumptions/Limitations:
 *   The user should free the memory allocated by this function for the
 *   encoded buffer.
 *
 ****************************************************************************/
int base64_encode(const char *text, char **encoded_data, size_t *encoded_len);

/****************************************************************************
 * Name: base64_decode
 *
 * Description:
 *   This method decodes the base64 URL input buffer into a NULL terminated
 *   ASCII text. It allocates memory for the decoded buffer.
 *
 * Input Parameters:
 *   decoded_text    - [out] here we store the decoded Base64 URL string
 *   decoded_len     - [out] location where we store the size of the
 *                    encoded_data
 *   encoded_data    - [in] the encoded Base64URL buffer
 *
 * Returned Value:
 *   OK in case of success otherwise a negative error code.
 *
 * Assumptions/Limitations:
 *   The user should free the memory allocated by this function for the
 *   decoded buffer.
 *
 ****************************************************************************/
int base64_decode(char **decoded_text, size_t *decoded_len,
  const char *encoded_data);

#endif /* __BASE64_ENCODE_H */
