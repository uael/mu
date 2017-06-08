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

/*!\file vector.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_VECTOR_H__
# define U_VECTOR_H__

#include "buffer.h"

#define T_uvec uvec

#define UVEC_MIN_CAPACITY 4

#define uvec_of(T) struct { \
    ds_super(T); \
  }

#define uvec_it(v) ds_it(v)

#define uvec_begin(v) ds_data(v)

#define uvec_end(v) (ds_data(v)+ds_size(v))

#define uvec_growth(vector, nmin) \
  ds_growth(vector, nmin, sizeof(*ds_data(vector)))

#define uvec_decay(vector, nmax) \
  ds_decay(vector, nmax, sizeof(*ds_data(vector)))

#define uvec_grow(vector, nmemb) \
  ds_grow(vector, (nmemb), sizeof(*ds_data(vector)))

#define uvec_shrink(vector, nmemb) \
  ds_shrink(vector, (nmemb), sizeof(*ds_data(vector)))

#define uvec_dtor(v) \
    ((ds_size(v) = ds_cap(v) = 0), \
    (ds_data(v) ? free(ds_data(v)) : (void) 0), \
    ds_data(v) = nullptr)

#define uvec_resize(vector, num) \
  (ds_size(vector) = uvec_growth(vector, num))

#define uvec_copy(dst, src) \
  ( \
    ( \
      sizeof(*ds_data(src)) == sizeof(*ds_data(dst)) \
    ) ? ( \
      buf_reserve(dst, ds_size(src)), \
      memcpy(ds_data(dst), ds_data(src), (ds_size(src)) * sizeof(*ds_data(src))), \
      (ds_size(dst) = ds_size(src)) \
    ) : \
      uvec_clear(dst) \
  )

#define uvec_erase(vector, pos) \
  (memmove( \
    ds_data(vector) + (pos), ds_data(vector) + (pos) + 1, --ds_size(vector) * sizeof(*ds_data(vector)) \
  ), ds_size(vector))

#define uvec_erase_range(vector, pos, num) \
  (memmove(ds_data(vector) + (pos), ds_data(vector) + (pos) + (num), \
    (ds_size(vector) -= (num)) * sizeof(*ds_data(vector))), \
  ds_size(vector))

#define uvec_ppush(v) \
  (uvec_grow(v, 1), ds_pat(v, ds_size(v)++))

#define uvec_push(v, x) \
  ((*uvec_ppush(v) = (x)), ds_pat(v, ds_size(v)-1))

#define uvec_insert(vector, pos, element) \
  (uvec_grow(vector, 1), \
  memmove(ds_data(vector) + (pos) + 1, ds_data(vector) + (pos), \
    sizeof(*ds_data(vector)) * (ds_size(vector)++ - (pos))), \
  ds_at(vector, (pos)) = (element))

#define uvec_pop(v) ds_data(v)[--ds_size(v)]

#define uvec_unshift(v, x) ( \
    uvec_grow(v, 1), \
    memmove( \
      ds_data(v) + 1, \
      ds_data(v), \
      ds_size(v)++ * sizeof(*ds_data(v)) \
    ), \
    ds_data(v)[0] = (x) \
  )

#define uvec_shift(v) \
  uvec_erase(v, 0)

#define uvec_clear(v) ( \
    (ds_data(v) ? memset(ds_data(v), 0, ds_size(v)) : 0), \
    ds_size(v) = 0 \
  )

#endif /* U_VECTOR_H__ */
