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

/*!\file array.h
 * \author Mattias Jansson <www.github.com/rampantpixels>
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_ARRAY_H__
# define U_ARRAY_H__

#include "types.h"

/*! \internal Header size set to 16 bytes in order to align main array memory */
#define UARR_HEADER_SIZE 8UL
#define UARR_MIN_CAPACITY 4
#define ROUNDUP32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

/*! \internal Access the raw array header area */
#define UARR_RAW(a) ((size_t*)(a) - UARR_HEADER_SIZE)
#define UARR_RAW_CONST(a) ((const size_t*)(a) - UARR_HEADER_SIZE)
#define UARR_RAW_CAPACITY(a) UARR_RAW(a)[0]
#define UARR_RAW_SIZE(a) UARR_RAW(a)[1]
#define UARR_RAW_CONST_CAPACITY(a) UARR_RAW_CONST(a)[0]
#define UARR_RAW_CONST_SIZE(a) UARR_RAW_CONST(a)[1]
#define UARR_VERIFY_INDEX(a,n) ((size_t)(n) < UARR_RAW_SIZE(a))
#define UARR_GROWUP(a, n) __uarr_growup((void **) &(a), n, sizeof(*(a)))
#define UARR_RESIZE(a,n) __uarr_resize((void **) &(a), (n), sizeof(*(a)))

/*!\def   uarr_dtor
 * \brief Deallocate array memory and reset array pointer to zero. Value of expression
 *        is a null pointer.
 * \param array Array pointer
 */
#define uarr_dtor(array) \
  ((array) ? (free(UARR_RAW(array)), ((array) = 0)) : 0)

/*!\def   uarr_capacity
 * \brief Get capacity of array in number of elements. Capacity indicates the size of the allocated
 *        memory block (maximum size of array).
 * \param array Array pointer
 */
#define uarr_capacity(array) \
  ((array) ? UARR_RAW_CONST_CAPACITY(array) : 0)

/*!\def   uarr_reserve
 * \brief Reserve storage for given number of elements. Never reduces storage and does not affect
 *        number of currently stored elements.
 * \param array    Array pointer
 * \param capacity New capacity
 */
#define uarr_reserve(array, capacity) \
  UARR_GROWUP(array, (capacity) - uarr_capacity(array))

/*!\def   uarr_size
 * \brief Get number of currently stored elements.
 * \param array Array pointer
 */
#define uarr_size(array) \
  ((array) ? UARR_RAW_CONST_SIZE(array) : 0)

/*!\def   uarr_grow
 * \brief Add or remove elements without initialization, if size is positive or negative respectively.
 *        Sets new size to uarr_size(array)+num and allocates new storage if new size is larger than
 *        array capacity. Value of expression is the new size of the array.
 * \param array Array pointer
 * \param num   Number of elements to grow/shrink
 */
#define uarr_grow(array, num) \
  UARR_RESIZE(array, uarr_size(array) + (ssize_t) (num))

/*!\def   uarr_resize
 * \brief Resize to given absolute size without initialization. Sets new size to num and allocate
 *        new storage if new size is larger than array capacity.
 * \param array Array pointer
 * \param num   New size
 */
#define uarr_resize(array, num) \
  UARR_RESIZE(array, num)

/*!\def   uarr_clear
 * \brief Set size to 0. Does not affect capacity or resize array storage buffer.
 *        Reset all memory
 * \param array Array pointer
 */
#define uarr_clear(array) \
  ((array) ? (memset((array), 0, UARR_RAW_SIZE(array)), UARR_RAW_SIZE(array) = 0) : 0)

/*!\def   uarr_copy
 * \brief Copy content of one array to another, setting new destination array size to source array
 *        size and allocating more storage if new destination size is larger than destination array
 *        capacity. Value of the expression is the size of the destination array after copy.
 * \param dst Destination array
 * \param src Source array
 */
#define uarr_copy(dst, src) \
  ( \
    ( \
      (src) && \
      sizeof(*(src)) == sizeof(*(dst)) && \
      (UARR_GROWUP((dst), UARR_RAW_CONST_SIZE(src) - ((dst) ? (UARR_RAW_SIZE(dst)) : 0)), (dst)) \
    ) ? ( \
      memcpy((dst), (src), (UARR_RAW_CONST_SIZE(src)) * sizeof(*(src))), \
      UARR_RAW_SIZE(dst) = UARR_RAW_CONST_SIZE(src) \
    ) : \
      uarr_clear(dst) \
  )

/*!\def   uarr_push
 * \brief Add element at end of array with assignment. Value of expression is new array pointer.
 * \param array   Array pointer
 * \param element New element
 */
#define uarr_push(array, element) \
  (UARR_GROWUP(array, 1), (array)[UARR_RAW_SIZE(array)++] = (element))

/*!\def   uarr_push_memcpy
 * \brief Add element at end of array copying data with memcpy. Value of expression
 *        is new array pointer.
 * \param array       Array pointer
 * \param element_ptr Pointer to new element
 */
#define uarr_push_memcpy(array, element_ptr) \
  (UARR_GROWUP(array, 1), (array) ? \
    (memcpy((array) + UARR_RAW_SIZE(array)++, (element_ptr), sizeof(*(array))), (array)) : \
    (array))

/*!\def   uarr_insert
 * \brief Add element at given position in array with assignment. Position is range checked.
 *        Existing elements are moved using memmove. Value of expression is new array pointer.
 * \param array   Array pointer
 * \param pos     Position
 * \param element New element
 */
#define uarr_insert(array, pos, element) \
  ((pos) >= 0 ? \
    UARR_GROWUP(array, (array) ? ((pos) < UARR_RAW_SIZE(array) ? 1 : ((pos) > 0 ? (pos) : 1)) : ((pos) > 0 ? (pos) : 1)), \
    (memmove((array) + (pos) + 1, (array) + (pos), \
      sizeof(*(array)) * (UARR_RAW_SIZE(array)++ - (pos))), \
    (array)[(pos)] = (element), (array)) : \
    (array))

/*!\def   uarr_insert_memcpy
 * \brief Add element at given position in array, copy data using memcpy. Position is range
 *        checked.
 *        Existing elements are moved using memmove. Value of expression is new array pointer.
 * \param array       Array pointer
 * \param pos         Position
 * \param element_ptr Pointer to new element
 */
#define uarr_insert_memcpy(array, pos, element_ptr) \
  ((pos) >= 0 ? \
    UARR_GROWUP(array, (array) ? ((pos) < UARR_RAW_SIZE(array) ? 1 : (pos)) : (pos)), \
    (memmove((array) + (pos) + 1, (array) + (pos), \
      sizeof(*(array)) * (UARR_RAW_SIZE(array)++ - (pos))), \
    memcpy((array) + (pos), (element_ptr), sizeof(*(array))), (array)) : \
    (array))

/*!\def   uarr_pop
 * \brief Remove last element. Array size is NOT validated, will cause undefined behaviour if
 *        called on an empty array.
 * \param array Array pointer
 */
#define uarr_pop(array) \
  ((array)[--UARR_RAW_SIZE(array)])

/*!\def   uarr_pop_safe
 * \brief Remove last element. Array size IS validated, safe to call on an empty array.
 * \param array Array pointer
 */
#define uarr_pop_safe(array) \
  ((array) && UARR_RAW_SIZE(array) > 0 ? --UARR_RAW_SIZE(array) : 0)

/*!\def   uarr_erase
 * \brief Erase element at given position without preserving order, swap-with-last using assignment.
 *        Position is NOT ranged checked.
 * \param array Array pointer
 * \param pos   Position
 */
#define uarr_erase(array, pos) \
  ((array) ? \
    *((array) + (pos)) = *((array) + (UARR_RAW_SIZE(array) - 1)), \
      --UARR_RAW_SIZE(array) : \
    0)

/*!\def   uarr_erase_memcpy
 * \brief Erase element at given position without preserving order, swap-with-last by copy data
 *        using memcpy. Position is NOT ranged checked.
 * \param array Array pointer
 * \param pos   Position
 */
#define uarr_erase_memcpy(array, pos) \
  ((array) ? \
    ( \
      ((size_t)(pos) != (UARR_RAW_SIZE(array) - 1) ? \
        memcpy((array) + (pos), (array) + (UARR_RAW_SIZE(array) - 1), sizeof(*(array))) : \
        0), \
        --UARR_RAW_SIZE(array) \
    ) : \
    0)

/*!\def   uarr_erase_safe
 * \brief Erase element at given position without preserving order, swap-with-last using assignment.
 *        Position IS ranged checked.
 * \param array Array pointer
 * \param pos   Position
 */
#define uarr_erase_safe(array, pos) \
  ((array) && UARR_VERIFY_INDEX(array, pos) ? \
    uarr_erase(array, pos) : \
    0)

/*!\def   uarr_erase_memcpy_safe
 * \brief Erase element at given position without preserving order, swap-with-last by copy data
 *        using memcpy. Position IS ranged checked.
 * \param array Array pointer
 * \param pos   Position
 */
#define uarr_erase_memcpy_safe(array, pos) \
  ((array) && UARR_VERIFY_INDEX( array, pos ) ? \
    uarr_erase_memcpy(array, pos) : \
    0)

/*!\def   uarr_erase_ordered
 * \brief Erase element at given position and preserve order by memmove remaining elements in array.
 *        Position is NOT ranged checked.
 * \param array Array pointer
 * \param pos   Position
 */
#define uarr_erase_ordered(array, pos) \
  ((array) ? \
    (memmove((array) + (pos), (array) + (pos) + 1, \
        (UARR_RAW_SIZE(array) - (pos) - 1U) * sizeof(*(array))), \
      --UARR_RAW_SIZE( array )) : \
    0)

/*!\def   uarr_erase_ordered_safe
 * \brief Erase element at given position and preserve order by memmove remaining elements in array.
 *        Position IS ranged checked.
 * \param array Array pointer
 * \param pos   Position
 */
#define uarr_erase_ordered_safe(array, pos) \
  ((array) && UARR_VERIFY_INDEX(array, pos) ? \
    uarr_erase_ordered(array, (size_t)pos) : \
    0)

/*!\def   uarr_erase_ordered_range
 * \brief Erase a range of elements at given position and preserve order by memmove remaining elements
 *        in array. Position and number of elements are NOT ranged checked
 * \param array Array pointer
 * \param pos   Starting position
 * \param num   Number of elements to erase
 */
#define uarr_erase_ordered_range(array, pos, num) \
  ((array) && \
    (/*lint -e506 */(num) > 0) ? \
      memmove((array) + (pos), (array) + (pos) + (num), \
          (UARR_RAW_SIZE(array) - (pos) - (num)) * sizeof(*(array))), \
        (UARR_RAW_SIZE(array) -= (num)) : \
      0)

/*!\def   uarr_erase_ordered_range_safe
 * \brief Erase a range of elements at given position and preserve order by memmove remaining elements
 *        in array. Position and number of elements ARE ranged checked
 * \param array Array pointer
 * \param pos   Starting position
 * \param num   Number of elements to erase
 */
#define uarr_erase_ordered_range_safe(array, pos, num) do { \
    size_t __s = uarr_size(array); \
    size_t __clamped_start = ((size_t)(pos) & 0x80000000U) ? \
      0 : math_min((size_t)(pos), __s); \
    size_t __clamped_end = ((size_t)((pos) + (num)) & 0x80000000U) ? \
      0 : math_min((size_t)((pos) + (num)), __s); \
    if (__clamped_end > __clamped_start) \
      uarr_erase_ordered_range(array, __clamped_start, __clamped_end - __clamped_start); \
  } while(0)


void __uarr_growup(void **arr, size_t n, size_t item_size);
void __uarr_resize(void **arr, size_t size, size_t item_size);

#endif /* U_ARRAY_H__ */
