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

#ifndef  U_TEST_H__
# define U_TEST_H__

#include <assert.h>
#include <time.h>
#include <stdio.h>

#include "u/compiler.h"

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

#define TEST_DATA struct test

typedef TEST_DATA test_t;

#define TEST_SETUP void test_setup(test_t *self)
#define TEST_TEARDOWN void test_teardown(test_t *self)

TEST_SETUP;
TEST_TEARDOWN;

FORCEINLINE CONSTCALL const char *test_run(test_t *self, const char *(*test_fn)(test_t *)) {
  const char *result;

  test_setup(self);
  result = test_fn(self);
  test_teardown(self);

  return result;
}

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ ":" S2(__LINE__)
#define ASSERT(expr) do { if (!(expr)) { return LOCATION " -> " #expr; } } while(false)
#define TESTFN(suite, name) suite ## _ ## name ## _test
#define TEST(suite, name) static const char *TESTFN(suite, name)(test_t *self)
#define TEST_PADDING "..................................."
#define TEST_RUN(suite, name) do { \
    const char *__result; \
    int __s = sizeof(TEST_PADDING) - sizeof(#suite ":" #name) - 2; \
    printf("Test:     %s %*.*s   ", #suite ":" #name, __s, __s, TEST_PADDING); \
    if ((__result = test_run(&test, TESTFN(suite, name)))) { \
      printf(RED "[FAILED] ‘%s’" RESET "\n", __result); \
      return EXIT_FAILURE; \
    } \
    puts(GREEN "[OK]" RESET); \
  } while (false) \

#endif /* U_TEST_H__ */
