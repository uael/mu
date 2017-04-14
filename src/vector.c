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

#include <u/vector.h>
#include <assert.h>

#define IS_POWER_OF2(n) ((n & (n - 1)) == 0)

void __uvec_growup(void **data, size_t *capacity, size_t size, size_t n, size_t item_size) {
  if (n) {
    if (*capacity) {
      if (*capacity > size + n) {
        return;
      }
      if (IS_POWER_OF2(size + n)) {
        *capacity = size + n;
      } else {
        do *capacity *= 2; while(*capacity <= size + n);
      }
      *data = realloc(*data, item_size * *capacity);
    } else {
      if (n == UVEC_MIN_CAPACITY || (n > UVEC_MIN_CAPACITY && IS_POWER_OF2(n))) {
        *capacity = n;
      } else {
        *capacity = UVEC_MIN_CAPACITY;
        while (*capacity <= n) *capacity *= 2;
      }
      *data = malloc(item_size * *capacity);
    }
  }
  return;
}

void __uvec_resize(void **arr, size_t *capacity, size_t *size, size_t new_size, size_t item_size) {
  assert(size);
  if (!(*capacity) && new_size) {
    __uvec_growup(arr, capacity, *size, new_size, item_size);
  }
  else if (*arr && *capacity < new_size) {
    __uvec_growup(arr, capacity, *size, new_size - *size, item_size);
  }
  if (*capacity) {
    *size = new_size;
  }
}


bool uvec_grow_size(size_t *capacity, size_t expected) {
  if (!expected) {
    return false;
  }
  if (*capacity < UVEC_MIN_CAPACITY) {
    *capacity = UVEC_MIN_CAPACITY;
    if (expected >= *capacity) {
      do *capacity *= 2; while (*capacity <= expected);
    }
    return true;
  }
  if (expected >= *capacity) {
    do *capacity *= 2; while (*capacity <= expected);
    return true;
  }
  return false;
}
