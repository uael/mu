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

#include "types.h"

#define UVEC_MIN_CAPACITY 4
#define UVEC_GROWUP(a, n) \
  __uvec_growup((void **) &uvec_data(a), &uvec_capacity(a), uvec_size(a), (n), sizeof(*uvec_data(a)))
#define UVEC_RESIZE(a, n) \
  __uvec_resize((void **) &uvec_data(a), &uvec_capacity(a), &uvec_size(a), (n), sizeof(*uvec_data(a)))

#define uvec_of(t) struct { \
    size_t size, capacity; t *data; \
  }

/*!\def   uvec_size
 * \brief Get number of currently stored elements.
 * \param vector Vector structure
 */
#define uvec_size(v) (v).size

/*!\def   uvec_capacity
 * \brief Get capacity of vector in number of elements. Capacity indicates the size of the allocated
 *        memory block (maximum size of vector).
 * \param vector Vector structure
 */
#define uvec_capacity(v) (v).capacity

#define uvec_data(v) (v).data

#define uvec_length(v) uvec_size(v)

#define uvec_at(v, i) uvec_data(v)[i]

#define uvec_front(v) uvec_at(v, 0)

#define uvec_back(v) uvec_at(v, uvec_size(v)-1)

/*!\def   uvec_dtor
 * \brief Deallocate vector memory and reset vector pointer to zero. Value of expression
 *        is a null pointer.
 * \param vector Vector structure
 */
#define uvec_dtor(v) \
    ((uvec_size(v) = uvec_capacity(v) = 0), \
    (uvec_data(v) ? free(uvec_data(v)) : (void) 0), \
    uvec_data(v) = nullptr)

/*!\def   uvec_grow
 * \brief Add or remove elements without initialization, if size is positive or negative respectively.
 *        Sets new size to uvec_size(vector)+num and allocates new storage if new size is larger than
 *        vector capacity. Value of expression is the new size of the vector.
 * \param vector Vector structure
 * \param num    Number of elements to grow/shrink
 */
#define uvec_grow(vector, num) \
  UVEC_RESIZE(vector, uvec_size(vector) + (ssize_t) (num))

/*!\def   uvec_reserve
 * \brief Reserve storage for given number of elements. Never reduces storage and does not affect
 *        number of currently stored elements.
 * \param vector   Vector structure
 * \param capacity New capacity
 */
#define uvec_reserve(vector, capacity) \
  UVEC_GROWUP(vector, (capacity) - uvec_capacity(vector))

/*!\def   uvec_resize
 * \brief Resize to given absolute size without initialization. Sets new size to num and allocate
 *        new storage if new size is larger than vector capacity.
 * \param vector Vector structure
 * \param num    New size
 */
#define uvec_resize(vector, num) \
  UVEC_RESIZE(vector, num)

/*!\def   uvec_copy
 * \brief Copy content of one vector to another, setting new destination vector size to source vector
 *        size and allocating more storage if new destination size is larger than destination vector
 *        capacity. Value of the expression is the size of the destination vector after copy.
 * \param dst Destination vector
 * \param src Source vector
 */
#define uvec_copy(dst, src) \
  ( \
    ( \
      sizeof(*uvec_data(src)) == sizeof(*uvec_data(dst)) \
    ) ? ( \
      UVEC_GROWUP(dst, uvec_size(src) - uvec_size(dst)), \
      memcpy(uvec_data(dst), uvec_data(src), (uvec_size(src)) * sizeof(*uvec_data(src))), \
      (uvec_size(dst) = uvec_size(src)) \
    ) : \
      uvec_clear(dst) \
  )

/*!\def   uvec_erase
 * \brief Erase element at given position by preserving order, replace given position by its next.
 *        Position is NOT ranged checked.
 * \param vector Vector structure
 * \param pos    Position
 */
#define uvec_erase(vector, pos) \
  (memmove( \
    uvec_data(vector) + (pos), uvec_data(vector) + (pos) + 1, --uvec_size(vector) * sizeof(*uvec_data(vector)) \
  ), uvec_size(vector))

/*!\def   uvec_erase_safe
 * \brief Erase element at given position without preserving order, swap-with-last using assignment.
 *        Position IS ranged checked.
 * \param vector Vector structure
 * \param pos    Position
 */
#define uvec_erase_safe(vector, pos) \
  ((pos) < uvec_size(vector) ? \
    uvec_erase(vector, pos) : \
    uvec_size(vector))

/*!\def   uvec_erase_range
 * \brief Erase a range of elements at given position and preserve order by memmove remaining elements
 *        in vector. Position and number of elements are NOT ranged checked
 * \param vector Vector structure
 * \param pos    Starting position
 * \param num    Number of elements to erase
 */
#define uvec_erase_range(vector, pos, num) \
  (memmove(uvec_data(vector) + (pos), uvec_data(vector) + (pos) + (num), \
    (uvec_size(vector) -= (num)) * sizeof(*uvec_data(vector))), \
  uvec_size(vector))

/*!\def   uvec_erase_range_safe
 * \brief Erase a range of elements at given position and preserve order by memmove remaining elements
 *        in vector. Position and number of elements are ranged checked
 * \param vector Vector structure
 * \param pos    Starting position
 * \param num    Number of elements to erase
 */
#define uvec_erase_range_safe(vector, pos, num) \
  ((pos) + (num) < uvec_size(vector) ? \
    uvec_erase_range(vector, pos, num) : \
    uvec_size(vector))

/*!\def   uvec_push
 * \brief Add element at end of vector with assignment. Value of expression is new vector pointer.
 * \param vector   Vector structure
 * \param element  New element
 */
#define uvec_push(v, x) \
  (UVEC_GROWUP(v, 1), uvec_data(v)[uvec_size(v)++] = (x))

/*!\def   uvec_push_memcpy
 * \brief Add element at end of vector copying data with memcpy. Value of expression
 *        is new vector pointer.
 * \param vector      Vector structure
 * \param element_ptr Pointer to new element
 */
#define uvec_push_memcpy(vector, element_ptr) \
  (UVEC_GROWUP(vector, 1), memcpy(uvec_data(vector) + uvec_size(vector)++, (element_ptr), sizeof(*uvec_data(vector))))

/*!\def   uvec_insert
 * \brief Add element at given position in vector with assignment. Position is NOT range checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector  Vector structure
 * \param pos     Position
 * \param element New element
 */
#define uvec_insert(vector, pos, element) \
  (UVEC_GROWUP(vector, 1), \
  memmove(uvec_data(vector) + (pos) + 1, uvec_data(vector) + (pos), \
    sizeof(*uvec_data(vector)) * (uvec_size(vector)++ - (pos))), \
  uvec_at(vector, (pos)) = (element))

/*!\def   uvec_insert_memcpy
 * \brief Add element at given position in vector, copy data using memcpy. Position is NOT range
 *        checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector      Vector structure
 * \param pos         Position
 * \param element_ptr Pointer to new element
 */
#define uvec_insert_memcpy(vector, pos, element_ptr) \
  (UVEC_GROWUP(vector, 1), \
  memmove(uvec_data(vector) + (pos) + 1, uvec_data(vector) + (pos), \
    sizeof(*(vector)) * (uvec_size(vector)++ - (pos))), \
  memcpy(uvec_data(vector) + (pos), (element_ptr), sizeof(*uvec_datavector)))

/*!\def   uvec_insert_safe
 * \brief Add element at given position in vector with assignment. Position is range checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector  Vector structure
 * \param pos     Position
 * \param element New element
 */
#define uvec_insert_safe(vector, pos, element) \
  ((pos) && (pos) < uvec_size(vector) ? (uvec_insert(vector, pos, element), 0) : 0)

/*!\def   uvec_insert_safe_memcpy
 * \brief Add element at given position in vector, copy data using memcpy. Position is range
 *        checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector      Vector structure
 * \param pos         Position
 * \param element_ptr Pointer to new element
 */
#define uvec_insert_safe_memcpy(vector, pos, element_ptr) \
  ((pos) && (pos) < uvec_size(vector) ? (uvec_insert_memcpy(vector, pos, element_ptr), 0) : 0)

/*!\def   uvec_pop
 * \brief Remove last element. Vector size is NOT validated, will cause undefined behaviour if
 *        called on an empty vector.
 * \param vector Vector structure
 */
#define uvec_pop(v) uvec_data(v)[--uvec_size(v)]

/*!\def   uvec_pop_safe
 * \brief Remove last element. Vector size IS validated, safe to call on an empty vector.
 * \param vector Vector structure
 */
#define uvec_pop_safe(vector) \
  (uvec_size(vector) > 0 ? --uvec_size(vector) : 0)

#define uvec_unshift(v, x) ( \
    UVEC_GROWUP(v, 1), \
    memmove( \
      uvec_data(v) + 1, \
      uvec_data(v), \
      uvec_size(v)++ * sizeof(*uvec_data(v)) \
    ), \
    uvec_data(v)[0] = (x) \
  )

#define uvec_shift(v) \
  uvec_erase(v, 0)

/*!\def   uvec_clear
 * \brief Set size to 0. Does not affect capacity or resize vector storage buffer.
 *        Reset all memory.
 * \param vector Vector structure
 */
#define uvec_clear(v) ( \
    (uvec_data(v) ? memset(uvec_data(v), 0, uvec_size(v)) : 0), \
    uvec_size(v) = 0 \
  )

bool uvec_grow_size(size_t *capacity, size_t expected);
void __uvec_growup(void **data, size_t *capacity, size_t size, size_t n, size_t item_size);
void __uvec_resize(void **arr, size_t *capacity, size_t *size, size_t new_size, size_t item_size);

#endif /* U_VECTOR_H__ */
