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

#ifndef  U_CTEST_H__
# define U_CTEST_H__

#include <assert.h>
#include <time.h>
#include <stdio.h>

#include "u/compiler.h"
#include "u/string.h"
#include "u/types.h"

#if PLATFORM_POSIX
# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"
# define BOLD    "\033[1m"
#else
# define RESET
# define BLACK
# define RED
# define GREEN
# define YELLOW
# define BLUE
# define MAGENTA
# define CYAN
# define WHITE
# define BOLD
#endif

#define CTEST_DATA struct ctest
typedef CTEST_DATA ctest_t;

#define CTEST_SETUP void test_setup(ctest_t *self)
#define CTEST_TEARDOWN void test_teardown(ctest_t *self)

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ ":" S2(__LINE__)
#define ASSERT(expr) do if (!(expr)) return LOCATION " -> " #expr; while (false)

#define CTEST_PADDING "................................."
#define CTEST_FN(suite, name) suite ## _ ## name ## _test
#define CTEST_RUN(suite, name) \
  do if (test_run(&test, #suite ":" #name, sizeof(#suite ":" #name)-1, CTEST_FN(suite, name)) == EXIT_FAILURE) \
      return EXIT_FAILURE; \
  while (false)
#define CTEST(suite, name) static const char *CTEST_FN(suite, name)(ctest_t *self)

CTEST_SETUP;
CTEST_TEARDOWN;

FORCEINLINE CONSTCALL int test_run(ctest_t *self, const char *id, unsigned id_len, const char *(*test_fn)(ctest_t *)) {
  const char *result;
  int s = sizeof(CTEST_PADDING) - id_len - 1;

  printf("Test:     %s %*.*s   ", id, s, s, CTEST_PADDING);
  test_setup(self);
  result = test_fn(self);
  test_teardown(self);
  if (result) {
    printf(RED "[FAILED] ‘%s’" RESET "\n", result);
    return EXIT_FAILURE;
  }
  puts(GREEN "[OK]" RESET);
  return EXIT_SUCCESS;
}

#endif /* U_CTEST_H__ */
