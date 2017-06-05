/*
 * MIT License
 *
 * Copyright (c) 2017 Lucas Abel <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "cute.h"

#include "u/string.h"

CUTEST_DATA {
  string_t x, y;
};

CUTEST_SETUP {
  self->x = nullptr;
  self->y = nullptr;
}

CUTEST_TEARDOWN {
  if (self->x) {
    string_free(self->x);
  }
  if (self->y) {
    string_free(self->y);
  }
}

CUTEST(string, s) {
  string_t x = string_new("foo"), y;

  ASSERT(string_len(x) == 3 && memcmp(x, "foo\0", 4) == 0);

  string_free(x);
  x = string_newlen("foo", 2);
  ASSERT(string_len(x) == 2 && memcmp(x, "fo\0", 3) == 0);

  x = string_cat(x, "bar");
  ASSERT(string_len(x) == 5 && memcmp(x, "fobar\0", 6) == 0);

  x = string_cpy(x, "a");
  ASSERT(string_len(x) == 1 && memcmp(x, "a\0", 2) == 0);

  x = string_cpy(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk");
  ASSERT(string_len(x) == 33 && memcmp(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk\0", 33) == 0);

  string_free(x);
  x = string_catprintf(string_empty(), "%d", 123);
  ASSERT(string_len(x) == 3 && memcmp(x, "123\0", 4) == 0);

  string_free(x);
  x = string_new("--");
  x = string_catfmt(x, "Hello %s World %I,%I--", "Hi!", LLONG_MIN, LLONG_MAX);
  ASSERT(string_len(x) == 60 &&
    memcmp(x, "--Hello Hi! World -9223372036854775808,"
      "9223372036854775807--", 60) == 0);

  string_free(x);
  x = string_new("--");
  x = string_catfmt(x, "%u,%U--", UINT_MAX, ULLONG_MAX);
  ASSERT(string_len(x) == 35 &&
    memcmp(x, "--4294967295,18446744073709551615--", 35) == 0);

  string_free(x);
  x = string_new(" x ");
  string_trim(x, " x");
  ASSERT(string_len(x) == 0);

  string_free(x);
  x = string_new(" x ");
  string_trim(x, " ");
  ASSERT(string_len(x) == 1 && x[0] == 'x');

  string_free(x);
  x = string_new("xxciaoyyy");
  string_trim(x, "xy");
  ASSERT(string_len(x) == 4 && memcmp(x, "ciao\0", 5) == 0);

  y = string_dup(x);
  string_range(y, 1, 1);
  ASSERT(string_len(y) == 1 && memcmp(y, "i\0", 2) == 0);

  string_free(y);
  y = string_dup(x);
  string_range(y, 1, -1);
  ASSERT(string_len(y) == 3 && memcmp(y, "iao\0", 4) == 0);

  string_free(y);
  y = string_dup(x);
  string_range(y, -2, -1);
  ASSERT(string_len(y) == 2 && memcmp(y, "ao\0", 3) == 0);

  string_free(y);
  y = string_dup(x);
  string_range(y, 2, 1);
  ASSERT(string_len(y) == 0 && memcmp(y, "\0", 1) == 0);

  string_free(y);
  y = string_dup(x);
  string_range(y, 1, 100);
  ASSERT(string_len(y) == 3 && memcmp(y, "iao\0", 4) == 0);

  string_free(y);
  y = string_dup(x);
  string_range(y, 100, 100);
  ASSERT(string_len(y) == 0 && memcmp(y, "\0", 1) == 0);

  string_free(y);
  string_free(x);
  x = string_new("foo");
  y = string_new("foa");
  ASSERT(string_cmp(x, y) > 0);

  string_free(y);
  string_free(x);
  x = string_new("bar");
  y = string_new("bar");
  ASSERT(string_cmp(x, y) == 0);

  string_free(y);
  string_free(x);
  x = string_new("aar");
  y = string_new("bar");
  ASSERT(string_cmp(x, y) < 0);

  string_free(y);
  string_free(x);
  x = string_newlen("\a\n\0foo\r", 7);
  y = string_catrepr(string_empty(), x, string_len(x));
  ASSERT(memcmp(y, "\"\\a\\n\\x00foo\\r\"", 15) == 0);

  {
    char *p;
    size_t step = 10, j, i;

    string_free(x);
    string_free(y);
    x = string_new("0");
    ASSERT(string_len(x) == 1 && string_avail(x) == 0);

    /* Run the test a few times in order to hit the first two
     * SDS header types. */
    for (i = 0; i < 10; i++) {
      size_t oldlen = string_len(x);
      x = string_MakeRoomFor(x, (size_t) step);
      int type = x[-1] & SDS_TYPE_MASK;

      ASSERT(string_len(x) == oldlen);
      if (type != SDS_TYPE_5) {
        ASSERT(string_avail(x) >= step);
        string_avail(x);
      }
      p = x + oldlen;
      for (j = 0; j < step; j++) {
        p[j] = (char) ('A' + j);
      }
      string_IncrLen(x, step);
    }
    ASSERT(memcmp("0ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ", x, 101) == 0);
    ASSERT(string_len(x) == 101);

    string_free(x);
  }
  return NULL;
}

int main(void) {
  CUTEST_DATA test = {0};

  CUTEST_PASS(string, s);
  return EXIT_FAILURE;
}
