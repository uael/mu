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
  ustr_t x, y;
};

CUTEST_SETUP {
  self->x = nullptr;
  self->y = nullptr;
}

CUTEST_TEARDOWN {
  if (self->x) {
    ustrfree(self->x);
  }
  if (self->y) {
    ustrfree(self->y);
  }
}

CUTEST(ustr, s) {
  ustr_t x = ustr("foo"), y;

  ASSERT(ustrlen(x) == 3 && memcmp(x, "foo\0", 4) == 0);

  ustrfree(x);
  x = ustrn("foo", 2);
  ASSERT(ustrlen(x) == 2 && memcmp(x, "fo\0", 3) == 0);

  x = ustrcat(x, "bar");
  ASSERT(ustrlen(x) == 5 && memcmp(x, "fobar\0", 6) == 0);

  x = ustrcpy(x, "a");
  ASSERT(ustrlen(x) == 1 && memcmp(x, "a\0", 2) == 0);

  x = ustrcpy(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk");
  ASSERT(ustrlen(x) == 33 && memcmp(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk\0", 33) == 0);

  ustrfree(x);
  x = ustrcatprintf(ustrempty(), "%d", 123);
  ASSERT(ustrlen(x) == 3 && memcmp(x, "123\0", 4) == 0);

  ustrfree(x);
  x = ustr("--");
  x = ustrcatfmt(x, "Hello %s World %I,%I--", "Hi!", LLONG_MIN, LLONG_MAX);
  ASSERT(ustrlen(x) == 60 &&
    memcmp(x, "--Hello Hi! World -9223372036854775808,"
      "9223372036854775807--", 60) == 0);

  ustrfree(x);
  x = ustr("--");
  x = ustrcatfmt(x, "%u,%U--", UINT_MAX, ULLONG_MAX);
  ASSERT(ustrlen(x) == 35 &&
    memcmp(x, "--4294967295,18446744073709551615--", 35) == 0);

  ustrfree(x);
  x = ustr(" x ");
  ustrtrim(x, " x");
  ASSERT(ustrlen(x) == 0);

  ustrfree(x);
  x = ustr(" x ");
  ustrtrim(x, " ");
  ASSERT(ustrlen(x) == 1 && x[0] == 'x');

  ustrfree(x);
  x = ustr("xxciaoyyy");
  ustrtrim(x, "xy");
  ASSERT(ustrlen(x) == 4 && memcmp(x, "ciao\0", 5) == 0);

  y = ustrdup(x);
  ustrrange(y, 1, 1);
  ASSERT(ustrlen(y) == 1 && memcmp(y, "i\0", 2) == 0);

  ustrfree(y);
  y = ustrdup(x);
  ustrrange(y, 1, -1);
  ASSERT(ustrlen(y) == 3 && memcmp(y, "iao\0", 4) == 0);

  ustrfree(y);
  y = ustrdup(x);
  ustrrange(y, -2, -1);
  ASSERT(ustrlen(y) == 2 && memcmp(y, "ao\0", 3) == 0);

  ustrfree(y);
  y = ustrdup(x);
  ustrrange(y, 2, 1);
  ASSERT(ustrlen(y) == 0 && memcmp(y, "\0", 1) == 0);

  ustrfree(y);
  y = ustrdup(x);
  ustrrange(y, 1, 100);
  ASSERT(ustrlen(y) == 3 && memcmp(y, "iao\0", 4) == 0);

  ustrfree(y);
  y = ustrdup(x);
  ustrrange(y, 100, 100);
  ASSERT(ustrlen(y) == 0 && memcmp(y, "\0", 1) == 0);

  ustrfree(y);
  ustrfree(x);
  x = ustr("foo");
  y = ustr("foa");
  ASSERT(ustrcmp(x, y) > 0);

  ustrfree(y);
  ustrfree(x);
  x = ustr("bar");
  y = ustr("bar");
  ASSERT(ustrcmp(x, y) == 0);

  ustrfree(y);
  ustrfree(x);
  x = ustr("aar");
  y = ustr("bar");
  ASSERT(ustrcmp(x, y) < 0);

  ustrfree(y);
  ustrfree(x);
  x = ustrn("\a\n\0foo\r", 7);
  y = ustrcatrepr(ustrempty(), x, ustrlen(x));
  ASSERT(memcmp(y, "\"\\a\\n\\x00foo\\r\"", 15) == 0);

  {
    char *p;
    size_t step = 10, j, i;

    ustrfree(x);
    ustrfree(y);
    x = ustr("0");
    ASSERT(ustrlen(x) == 1 && ustravail(x) == 7);

    /* Run the test a few times in order to hit the first two
     * SDS header types. */
    for (i = 0; i < 10; i++) {
      size_t oldlen = ustrlen(x);
      x = ustrgrow(x, (size_t) step);
      int type = x[-1];

      ASSERT(ustrlen(x) == oldlen);
      ASSERT(ustravail(x) >= step);
      p = x + oldlen;
      for (j = 0; j < step; j++) {
        p[j] = (char) ('A' + j);
      }
      ustrinclen(x, step);
    }
    ASSERT(memcmp("0ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ", x, 101) == 0);
    ASSERT(ustrlen(x) == 101);

    ustrfree(x);
  }
  return NULL;
}

int main(void) {
  CUTEST_DATA test = {0};

  CUTEST_PASS(ustr, s);
  return EXIT_SUCCESS;
}
