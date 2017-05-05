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

#include <u/ds.h>
#include <u/string.h>
#include <u/math.h>

size_t ds_pgrowth(ds_t *self, const ssize_t nmin, const size_t isize) {
  if (nmin > 0) {
    if (self->cap) {
      if (self->cap < nmin) {
        if (ISPOW2(nmin)) {
          self->cap = (size_t) nmin;
        } else {
          do self->cap *= 2; while(self->cap < nmin);
        }
        self->data = realloc(self->data, isize * self->cap);
      }
    } else {
      if (nmin == DS_MIN_CAP || (nmin > DS_MIN_CAP && ISPOW2(nmin))) {
        self->cap = (size_t) nmin;
      } else {
        self->cap = DS_MIN_CAP;
        while (self->cap < nmin) self->cap *= 2;
      }
      self->data = malloc(isize * self->cap);
    }
    return (size_t) nmin;
  }
  return 0;
}

API size_t ds_pdecay(ds_t *self, const ssize_t nmax, const size_t isize) {
  size_t nearest_pow2;

  if (nmax >= 0) {
    nearest_pow2 = roundup32((size_t) nmax);
    if (self->cap > nearest_pow2) {
      self->cap = nearest_pow2;
      self->data = realloc(self->data, isize * self->cap);
    }
    if (self->size > nmax) {
      memset((char *) self->data + nmax * isize, 0, (self->size - nmax) * isize);
    }
    return (size_t) nmax;
  }
  return 0;
}
