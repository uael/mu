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

#include <u/array.h>

#define IS_POWER_OF2(n) ((n & (n - 1)) == 0)

void __uarr_growup(void **arr, size_t n, size_t item_size) {
  size_t *raw, *buffer, capacity;

  if (n) {
    if (*arr) {
      raw = UARR_RAW(*arr);
      if (raw[0] > raw[1] + n) {
        return;
      }
      if (IS_POWER_OF2(raw[1] + n)) {
        raw[0] = raw[1] + n;
      } else {
        do raw[0] *= 2; while(raw[0] <= raw[1] + n);
      }
      buffer = realloc(raw, item_size * raw[0] + 8U * UARR_HEADER_SIZE);
    } else {
      if (n == UARR_MIN_CAPACITY || (n > UARR_MIN_CAPACITY && IS_POWER_OF2(n))) {
        capacity = n;
      } else {
        capacity = UARR_MIN_CAPACITY;
        while (capacity <= n) capacity *= 2;
      }
      buffer = malloc(item_size * capacity + 8U * UARR_HEADER_SIZE);
      buffer[0] = capacity;
      buffer[1] = 0;
    }
    *arr = buffer + UARR_HEADER_SIZE;
  }
  return;
}

void __uarr_resize(void **arr, size_t size, size_t item_size) {
  if (!(*arr) && size) {
    __uarr_growup(arr, size, item_size);
  }
  else if (*arr && UARR_RAW_CAPACITY(*arr) < size) {
    __uarr_growup(arr, size - UARR_RAW_SIZE(*arr), item_size);
  }
  if (*arr) {
    UARR_RAW_SIZE(*arr) = size;
  }
}
