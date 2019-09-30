#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "base64_encode.h"

static base64_encode_test_t g_tests[] = {
  {
    .plain_data 	= "Ce mai faci?",
    .encoded_data = "Q2UgbWFpIGZhY2k_"
  },
  {
    .plain_data   = "ABCDEFGHI",
    .encoded_data = "QUJDREVGR0hJ"
  },
  {
    .plain_data   = "Base64URL Encode is a free online tool for converting data to Base64 value which can be safely used for URLs and filenames. You can submit the data you want to encode to Base64URL by typing or pasting text, uploading a file, or specifying a URL",
    .encoded_data = "QmFzZTY0VVJMIEVuY29kZSBpcyBhIGZyZWUgb25saW5lIHRvb2wgZm9yIGNvbnZlcnRpbmcgZGF0YSB0byBCYXNlNjQgdmFsdWUgd2hpY2ggY2FuIGJlIHNhZmVseSB1c2VkIGZvciBVUkxzIGFuZCBmaWxlbmFtZXMuIFlvdSBjYW4gc3VibWl0IHRoZSBkYXRhIHlvdSB3YW50IHRvIGVuY29kZSB0byBCYXNlNjRVUkwgYnkgdHlwaW5nIG9yIHBhc3RpbmcgdGV4dCwgdXBsb2FkaW5nIGEgZmlsZSwgb3Igc3BlY2lmeWluZyBhIFVSTA"
  },
  {
    .plain_data   = "If you have any questions, remarks, need help, or just like this page, please feel free to let me know by leaving a comment using the form bellow.I will be happy to read every comment and,",
    .encoded_data = "SWYgeW91IGhhdmUgYW55IHF1ZXN0aW9ucywgcmVtYXJrcywgbmVlZCBoZWxwLCBvciBqdXN0IGxpa2UgdGhpcyBwYWdlLCBwbGVhc2UgZmVlbCBmcmVlIHRvIGxldCBtZSBrbm93IGJ5IGxlYXZpbmcgYSBjb21tZW50IHVzaW5nIHRoZSBmb3JtIGJlbGxvdy5JIHdpbGwgYmUgaGFwcHkgdG8gcmVhZCBldmVyeSBjb21tZW50IGFuZCw"
  },
  {
    .plain_data   = "if necessary, I will do my best to respond as quickly as possible. Of course, spammers are",
    .encoded_data = "aWYgbmVjZXNzYXJ5LCBJIHdpbGwgZG8gbXkgYmVzdCB0byByZXNwb25kIGFzIHF1aWNrbHkgYXMgcG9zc2libGUuIE9mIGNvdXJzZSwgc3BhbW1lcnMgYXJl"
  },
};

int main(int argc, char **argv)
{
  char *encoded_buffer = NULL;
  size_t encoded_buffer_len = 0;
  char *plain_text = NULL;
  size_t plain_text_len = 0;

  int ret = OK;

  fprintf(stdout, "**** ENCODING ****\n");
  /* Test encoding */
  for (int i = 0; i < LENGTH(g_tests); ++i)
  {
    ret = base64_encode(g_tests[i].plain_data, &encoded_buffer,
                        &encoded_buffer_len);

    assert(ret == OK);

    if (memcmp(encoded_buffer, g_tests[i].encoded_data, encoded_buffer_len) != 0 ||
        strlen(g_tests[i].encoded_data) != encoded_buffer_len) {
      fprintf(stderr, "\n[Test_%d] FAILED\n", i);

      fprintf(stdout, "differa: %s\n", g_tests[i].encoded_data);
      fprintf(stdout, "generat: %s\n", encoded_buffer);
    }
    else {
      fprintf(stdout, "[Test_%d] PASSED\n", i);
    }

    free(encoded_buffer);
  }

  fprintf(stdout, "**** DECODING ****\n");
  /* Test decoding */
  for (int i = 0; i < LENGTH(g_tests); ++i)
  {
    ret = base64_decode(&plain_text, &plain_text_len, g_tests[i].encoded_data);
    assert(ret == OK);

    if (memcmp(plain_text, g_tests[i].plain_data, plain_text_len) != 0 ||
        strlen(g_tests[i].plain_data) != plain_text_len) {

      fprintf(stderr, "\n[Test_%d] FAILED\n", i);

      fprintf(stdout, "differa: %s\n", g_tests[i].plain_data);
      fprintf(stdout, "generat: %s\n", plain_text);
    }
    else {
      fprintf(stdout, "[Test_%d] PASSED\n", i);
    }

    free(plain_text);
  }
  return ret;
}
