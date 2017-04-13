#include <u/array.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include <ctest/ctest.h>

typedef ALIGNED_STRUCT(_basic_type, 8) {
  int intval;
  float floatval;
  uint64_t objval;
} basic_t;

typedef ALIGNED_STRUCT(_combined_type, 8) {
  int intval;
  union {
    int intval;
    real realval;
    void *ptrval;
    basic_t basicval;
    uint64_t objval;
  } unionval;
  char charval;
  basic_t basicval;
  void *ptrval;
} combine_t;

CTEST(array, allocation) {
  void **uarr_ptr = 0;
  int *uarr_int = 0;
  uint64_t *uarr_obj = 0;
  basic_t *uarr_basic = 0;
  combine_t *uarr_combine = 0;

  ASSERT_EQUAL(0, uarr_size(uarr_ptr));
  ASSERT_EQUAL(0, uarr_size(uarr_int));
  ASSERT_EQUAL(0, uarr_size(uarr_obj));
  ASSERT_EQUAL(0, uarr_size(uarr_basic));
  ASSERT_EQUAL(0, uarr_size(uarr_combine));

  ASSERT_EQUAL(0, uarr_capacity(uarr_ptr));
  ASSERT_EQUAL(0, uarr_capacity(uarr_int));
  ASSERT_EQUAL(0, uarr_capacity(uarr_obj));
  ASSERT_EQUAL(0, uarr_capacity(uarr_basic));
  ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

  {
    uarr_reserve(uarr_ptr, 0);
    uarr_reserve(uarr_int, 0);
    uarr_reserve(uarr_obj, 0);
    uarr_reserve(uarr_basic, 0);
    uarr_reserve(uarr_combine, 0);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(0, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(0, uarr_capacity(uarr_int));
    ASSERT_EQUAL(0, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(0, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_ptr);
    ASSERT_EQUAL(0, uarr_int);
    ASSERT_EQUAL(0, uarr_obj);
    ASSERT_EQUAL(0, uarr_basic);
    ASSERT_EQUAL(0, uarr_combine);

    uarr_reserve(uarr_ptr, 32);
    uarr_reserve(uarr_int, 33);
    uarr_reserve(uarr_obj, 63);
    uarr_reserve(uarr_basic, 1024);
    uarr_reserve(uarr_combine, 32742);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(64, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(64, uarr_capacity(uarr_int));
    ASSERT_EQUAL(64, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(2048, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(32768, uarr_capacity(uarr_combine));
  }
// Deallocate
  {
    ASSERT_EQUAL(nullptr, uarr_dtor(uarr_ptr));
    ASSERT_EQUAL(nullptr, uarr_dtor(uarr_int));
    ASSERT_EQUAL(nullptr, uarr_dtor(uarr_obj));
    ASSERT_EQUAL(nullptr, uarr_dtor(uarr_basic));
    ASSERT_EQUAL(nullptr, uarr_dtor(uarr_combine));

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(0, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(0, uarr_capacity(uarr_int));
    ASSERT_EQUAL(0, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(0, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_ptr);
    ASSERT_EQUAL(0, uarr_int);
    ASSERT_EQUAL(0, uarr_obj);
    ASSERT_EQUAL(0, uarr_basic);
    ASSERT_EQUAL(0, uarr_combine);
  }
// Grow
  {
    uarr_grow(uarr_ptr, 75284);
    uarr_grow(uarr_int, 8295);
    uarr_grow(uarr_obj, 610);
    uarr_grow(uarr_basic, 24);
    uarr_grow(uarr_combine, 0);

    ASSERT_EQUAL(75284, uarr_size(uarr_ptr));
    ASSERT_EQUAL(8295, uarr_size(uarr_int));
    ASSERT_EQUAL(610, uarr_size(uarr_obj));
    ASSERT_EQUAL(24, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(131072, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(16384, uarr_capacity(uarr_int));
    ASSERT_EQUAL(1024, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(32, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    uarr_grow(uarr_ptr, 75284);
    uarr_grow(uarr_int, 8295);
    uarr_grow(uarr_obj, 610);
    uarr_grow(uarr_basic, 24);
    uarr_grow(uarr_combine, 0);

    ASSERT_EQUAL(75284 * 2, uarr_size(uarr_ptr));
    ASSERT_EQUAL(8295 * 2, uarr_size(uarr_int));
    ASSERT_EQUAL(610 * 2, uarr_size(uarr_obj));
    ASSERT_EQUAL(24 * 2, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(131072, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(16384, uarr_capacity(uarr_int));
    ASSERT_EQUAL(1024, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(64, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    uarr_grow(uarr_ptr, -1);
    uarr_grow(uarr_int, -2);
    uarr_grow(uarr_obj, -3);
    uarr_grow(uarr_basic, -4);
    uarr_grow(uarr_combine, 0);   //Grow is not bounds checked

    ASSERT_EQUAL(75284 * 2 - 1, uarr_size(uarr_ptr));
    ASSERT_EQUAL(8295 * 2 - 2, uarr_size(uarr_int));
    ASSERT_EQUAL(610 * 2 - 3, uarr_size(uarr_obj));
    ASSERT_EQUAL(24 * 2 - 4, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(262144, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(32768, uarr_capacity(uarr_int));
    ASSERT_EQUAL(2048, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(64, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_combine);
  }
// Clear
  {
    uarr_clear(uarr_ptr);
    uarr_clear(uarr_int);
    uarr_clear(uarr_obj);
    uarr_clear(uarr_basic);
    uarr_clear(uarr_combine);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(262144, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(32768, uarr_capacity(uarr_int));
    ASSERT_EQUAL(2048, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(64, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_combine);

    uarr_dtor(uarr_ptr);
    uarr_dtor(uarr_int);
    uarr_dtor(uarr_obj);
    uarr_dtor(uarr_basic);
    uarr_dtor(uarr_combine);

    uarr_clear(uarr_ptr);
    uarr_clear(uarr_int);
    uarr_clear(uarr_obj);
    uarr_clear(uarr_basic);
    uarr_clear(uarr_combine);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(0, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(0, uarr_capacity(uarr_int));
    ASSERT_EQUAL(0, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(0, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(0, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_ptr);
    ASSERT_EQUAL(0, uarr_int);
    ASSERT_EQUAL(0, uarr_obj);
    ASSERT_EQUAL(0, uarr_basic);
    ASSERT_EQUAL(0, uarr_combine);
  }
// Reserve + grow
  {
    void **uarr_ptr_prev = 0;
    int *uarr_int_prev = 0;
    uint64_t *uarr_obj_prev = 0;
    basic_t *uarr_basic_prev = 0;
    combine_t *uarr_combine_prev = 0;

    uarr_reserve(uarr_ptr, 1);
    uarr_reserve(uarr_int, 2);
    uarr_reserve(uarr_obj, 3);
    uarr_reserve(uarr_basic, 4);
    uarr_reserve(uarr_combine, 5);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(8, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(8, uarr_capacity(uarr_combine));

    ASSERT_NOT_EQUAL(uarr_ptr, 0);
    ASSERT_NOT_EQUAL(uarr_int, 0);
    ASSERT_NOT_EQUAL(uarr_obj, 0);
    ASSERT_NOT_EQUAL(uarr_basic, 0);
    ASSERT_NOT_EQUAL(uarr_combine, 0);

    uarr_ptr_prev = uarr_ptr;
    uarr_int_prev = uarr_int;
    uarr_obj_prev = uarr_obj;
    uarr_basic_prev = uarr_basic;
    uarr_combine_prev = uarr_combine;

    uarr_grow(uarr_ptr, 1);
    uarr_grow(uarr_int, 2);
    uarr_grow(uarr_obj, 3);
    uarr_grow(uarr_basic, 4);
    uarr_grow(uarr_combine, 5);

    ASSERT_EQUAL(1, uarr_size(uarr_ptr));
    ASSERT_EQUAL(2, uarr_size(uarr_int));
    ASSERT_EQUAL(3, uarr_size(uarr_obj));
    ASSERT_EQUAL(4, uarr_size(uarr_basic));
    ASSERT_EQUAL(5, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(8, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(8, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(uarr_ptr_prev, uarr_ptr);
    ASSERT_EQUAL(uarr_int_prev, uarr_int);
    ASSERT_EQUAL(uarr_obj_prev, uarr_obj);
    ASSERT_EQUAL(uarr_basic_prev, uarr_basic);
    ASSERT_EQUAL(uarr_combine_prev, uarr_combine);

    uarr_grow(uarr_ptr, 1);
    uarr_grow(uarr_int, 2);
    uarr_grow(uarr_obj, 3);
    uarr_grow(uarr_basic, 4);
    uarr_grow(uarr_combine, 5);

    ASSERT_EQUAL(1 * 2, uarr_size(uarr_ptr));
    ASSERT_EQUAL(2 * 2, uarr_size(uarr_int));
    ASSERT_EQUAL(3 * 2, uarr_size(uarr_obj));
    ASSERT_EQUAL(4 * 2, uarr_size(uarr_basic));
    ASSERT_EQUAL(5 * 2, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(8, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(8, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(8, uarr_capacity(uarr_combine));
  }

  uarr_dtor(uarr_ptr);
  uarr_dtor(uarr_int);
  uarr_dtor(uarr_obj);
  uarr_dtor(uarr_basic);
  uarr_dtor(uarr_combine);
}

CTEST(array, copy) {
  int i;
  void **uarr_ptr = 0;
  int *uarr_int = 0;
  uint64_t *uarr_obj = 0;
  basic_t *uarr_basic = 0;
  combine_t *uarr_combine = 0;

  void **copy_ptr = 0;
  int *copy_int = 0;
  uint64_t *copy_obj = 0;
  basic_t *copy_basic = 0;
  combine_t *copy_combine = 0;

  for (i = 0; i < 255; ++i) {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    basic.intval = i;
    basic.floatval = (float) i;
    basic.objval = (uint64_t) (i + 1);

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = i;
    combine.basicval.objval = (uint64_t) (i + 1);
    combine.intval = i + 2;
    combine.ptrval = 0;
    combine.charval = (char) i;
    combine.unionval.basicval.intval = i + 3;
    combine.unionval.basicval.objval = (uint64_t) (i + 4);

    uarr_push(uarr_ptr, (void *) ((uintptr_t) i));
    uarr_push(uarr_int, i);
    uarr_push(uarr_obj, (uint64_t) i);
    uarr_push(uarr_basic, basic);
    uarr_push(uarr_combine, combine);
  }

  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_copy(copy_ptr, uarr_ptr));
  ASSERT_EQUAL(uarr_size(uarr_int), uarr_copy(copy_int, uarr_int));
  ASSERT_EQUAL(uarr_size(uarr_obj), uarr_copy(copy_obj, uarr_obj));
  ASSERT_EQUAL(uarr_size(uarr_basic), uarr_copy(copy_basic, uarr_basic));
  ASSERT_EQUAL(uarr_size(uarr_combine), uarr_copy(copy_combine, uarr_combine));

  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_ptr));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_int));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_obj));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_basic));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uarr_ptr[i], copy_ptr[i]);
    ASSERT_EQUAL(uarr_int[i], copy_int[i]);
    ASSERT_EQUAL(uarr_obj[i], copy_obj[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic + i, uarr_basic + i, sizeof(*copy_basic)));
    ASSERT_EQUAL(0, memcmp(copy_combine + i, uarr_combine + i, sizeof(*copy_combine)));
  }

  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_copy(copy_ptr, uarr_ptr));
  ASSERT_EQUAL(uarr_size(uarr_int), uarr_copy(copy_int, uarr_int));
  ASSERT_EQUAL(uarr_size(uarr_obj), uarr_copy(copy_obj, uarr_obj));
  ASSERT_EQUAL(uarr_size(uarr_basic), uarr_copy(copy_basic, uarr_basic));
  ASSERT_EQUAL(uarr_size(uarr_combine), uarr_copy(copy_combine, uarr_combine));

  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_ptr));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_int));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_obj));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_basic));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uarr_ptr[i], copy_ptr[i]);
    ASSERT_EQUAL(uarr_int[i], copy_int[i]);
    ASSERT_EQUAL(uarr_obj[i], copy_obj[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic + i, uarr_basic + i, sizeof(*copy_basic)));
    ASSERT_EQUAL(0, memcmp(copy_combine + i, uarr_combine + i, sizeof(*copy_combine)));
  }

  for (i = 0; i < 255; ++i) {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    basic.intval = i;
    basic.objval = (uint64_t) (i + 1);

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = i;
    combine.basicval.objval = (uint64_t) (i + 1);
    combine.intval = i + 2;
    combine.ptrval = 0;
    combine.charval = (char) i;
    combine.unionval.basicval.intval = i + 3;
    combine.unionval.basicval.objval = (uint64_t) (i + 4);

    uarr_push(copy_ptr, (void *) ((uintptr_t) i));
    uarr_push(copy_int, i);
    uarr_push(copy_obj, (uint64_t) i);
    uarr_push(copy_basic, basic);
    uarr_push(copy_combine, combine);
  }

  uarr_erase(copy_ptr, 0);
  uarr_erase(copy_int, 0);
  uarr_erase(copy_obj, 0);
  uarr_erase(copy_basic, 0);
  uarr_erase(copy_combine, 0);

  uarr_copy(copy_ptr, uarr_ptr);
  uarr_copy(copy_int, uarr_int);
  uarr_copy(copy_obj, uarr_obj);
  uarr_copy(copy_basic, uarr_basic);
  uarr_copy(copy_combine, uarr_combine);

  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_ptr));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_int));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_obj));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_basic));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uarr_ptr[i], copy_ptr[i]);
    ASSERT_EQUAL(uarr_int[i], copy_int[i]);
    ASSERT_EQUAL(uarr_obj[i], copy_obj[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic + i, uarr_basic + i, sizeof(*copy_basic)));
    ASSERT_EQUAL(0, memcmp(copy_combine + i, uarr_combine + i, sizeof(*copy_combine)));
  }

  uarr_clear(copy_ptr);
  uarr_clear(copy_int);
  uarr_clear(copy_obj);
  uarr_clear(copy_basic);
  uarr_clear(copy_combine);

  uarr_copy(copy_ptr, uarr_ptr);
  uarr_copy(copy_int, uarr_int);
  uarr_copy(copy_obj, uarr_obj);
  uarr_copy(copy_basic, uarr_basic);
  uarr_copy(copy_combine, uarr_combine);

  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_ptr));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_int));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_obj));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_basic));
  ASSERT_EQUAL(uarr_size(uarr_ptr), uarr_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uarr_ptr[i], copy_ptr[i]);
    ASSERT_EQUAL(uarr_int[i], copy_int[i]);
    ASSERT_EQUAL(uarr_obj[i], copy_obj[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic + i, uarr_basic + i, sizeof(*copy_basic)));
    ASSERT_EQUAL(0, memcmp(copy_combine + i, uarr_combine + i, sizeof(*copy_combine)));
  }

  uarr_dtor(uarr_ptr);
  uarr_dtor(uarr_int);
  uarr_dtor(uarr_obj);
  uarr_dtor(uarr_basic);
  uarr_dtor(uarr_combine);

  uarr_dtor(copy_ptr);
  uarr_dtor(copy_int);
  uarr_dtor(copy_obj);
  uarr_dtor(copy_basic);
  uarr_dtor(copy_combine);
}

CTEST(array, pushpop) {
  int i, j;
  void **uarr_ptr = 0;
  int *uarr_int = 0;
  uint64_t *uarr_obj = 0;
  basic_t *uarr_basic = 0;
  combine_t *uarr_combine = 0;

  {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    basic.intval = 1;
    basic.objval = 2;

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = 3;
    combine.basicval.objval = 4;
    combine.charval = 5;
    combine.intval = 6;
    combine.ptrval = 0;
    combine.unionval.realval = REAL_C(1.0);

    ASSERT_EQUAL(0, uarr_push(uarr_ptr, 0));
    ASSERT_EQUAL(0, uarr_push(uarr_int, 0));
    ASSERT_EQUAL(0, uarr_push(uarr_obj, 0));
    uarr_push(uarr_basic, basic);
    uarr_push(uarr_combine, combine);

    ASSERT_EQUAL(1, uarr_size(uarr_ptr));
    ASSERT_EQUAL(1, uarr_size(uarr_int));
    ASSERT_EQUAL(1, uarr_size(uarr_obj));
    ASSERT_EQUAL(1, uarr_size(uarr_basic));
    ASSERT_EQUAL(1, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_ptr[0]);
    ASSERT_EQUAL(0, uarr_int[0]);
    ASSERT_EQUAL(0, uarr_obj[0]);
    ASSERT_EQUAL(0, memcmp(uarr_basic, &basic, sizeof(basic)));
    ASSERT_EQUAL(0, memcmp(uarr_combine, &combine, sizeof(combine)));
  }
// Pop
  {
    uarr_pop(uarr_ptr);
    uarr_pop(uarr_int);
    uarr_pop(uarr_obj);
    uarr_pop(uarr_basic);
    uarr_pop(uarr_combine);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));

    uarr_pop_safe(uarr_ptr);
    uarr_pop_safe(uarr_int);
    uarr_pop_safe(uarr_obj);
    uarr_pop_safe(uarr_basic);
    uarr_pop_safe(uarr_combine);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));
  }
// Push to non-empty
  for (i = 0; i < 255; ++i) {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    basic.intval = i;
    basic.objval = (uint64_t) (i + 1);

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = i;
    combine.basicval.objval = (uint64_t) (i + 1);
    combine.intval = i + 2;
    combine.ptrval = 0;
    combine.charval = (char) i;
    combine.unionval.basicval.intval = i + 3;
    combine.unionval.basicval.objval = (uint64_t) (i + 4);

    if (!(i % 2)) {
      uarr_push(uarr_ptr, (void *) ((uintptr_t) i));
      uarr_push(uarr_int, i);
      uarr_push(uarr_obj, (uint64_t) i);
      uarr_push(uarr_basic, basic);
      uarr_push(uarr_combine, combine);
    } else {
      void *iptr = (void *) ((uintptr_t) i);
      uint64_t iobj = (uint64_t) i;
      uarr_push_memcpy(uarr_ptr, &iptr);
      uarr_push_memcpy(uarr_int, &i);
      uarr_push_memcpy(uarr_obj, &iobj);
      uarr_push_memcpy(uarr_basic, &basic);
      uarr_push_memcpy(uarr_combine, &combine);
    }

    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_ptr));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_int));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_obj));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_basic));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_combine));

    for (j = 0; j <= i; ++j) {
      ASSERT_EQUAL((void *) ((uintptr_t) j), uarr_ptr[j]);
      ASSERT_EQUAL(j, uarr_int[j]);
      ASSERT_EQUAL((uint64_t) j, uarr_obj[j]);
      ASSERT_EQUAL(j, uarr_basic[j].intval);
      ASSERT_EQUAL((uint64_t) j + 1, uarr_basic[j].objval);
      ASSERT_EQUAL(j, uarr_combine[j].basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 1, uarr_combine[j].basicval.objval);
      ASSERT_EQUAL(j + 2, uarr_combine[j].intval);
      ASSERT_EQUAL((char) j, uarr_combine[j].charval);
      ASSERT_EQUAL(0, uarr_combine[j].ptrval);
      ASSERT_EQUAL(j + 3, uarr_combine[j].unionval.basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 4, uarr_combine[j].unionval.basicval.objval);
    }
  }
// Pop while non-empty
  for (; i > 0; --i) {
    uarr_pop(uarr_ptr);
    uarr_pop(uarr_int);
    uarr_pop(uarr_obj);
    uarr_pop(uarr_basic);
    uarr_pop(uarr_combine);

    ASSERT_EQUAL((unsigned int) i - 1, uarr_size(uarr_ptr));
    ASSERT_EQUAL((unsigned int) i - 1, uarr_size(uarr_int));
    ASSERT_EQUAL((unsigned int) i - 1, uarr_size(uarr_obj));
    ASSERT_EQUAL((unsigned int) i - 1, uarr_size(uarr_basic));
    ASSERT_EQUAL((unsigned int) i - 1, uarr_size(uarr_combine));

    for (j = 0; j < i; ++j) {
      ASSERT_EQUAL((void *) ((uintptr_t) j), uarr_ptr[j]);
      ASSERT_EQUAL(j, uarr_int[j]);
      ASSERT_EQUAL((uint64_t) j, uarr_obj[j]);
      ASSERT_EQUAL(j, uarr_basic[j].intval);
      ASSERT_EQUAL((uint64_t) j + 1, uarr_basic[j].objval);
      ASSERT_EQUAL(j, uarr_combine[j].basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 1, uarr_combine[j].basicval.objval);
      ASSERT_EQUAL(j + 2, uarr_combine[j].intval);
      ASSERT_EQUAL((char) j, uarr_combine[j].charval);
      ASSERT_EQUAL(0, uarr_combine[j].ptrval);
      ASSERT_EQUAL(j + 3, uarr_combine[j].unionval.basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 4, uarr_combine[j].unionval.basicval.objval);
    }
  }
// Interleaved push-pops
  for (i = 0; i < 255; ++i) {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    memset(&combine, 0, sizeof(combine));
    for (j = 0; j < 2; ++j) {
      basic.intval = i + j;
      basic.objval = (uint64_t) (i + j + 1);

      combine.basicval.intval = i + j;
      combine.basicval.objval = (uint64_t) (i + j + 1);
      combine.intval = i + j + 2;
      combine.ptrval = 0;
      combine.charval = (char) (i + j);
      combine.unionval.basicval.intval = i + j + 3;
      combine.unionval.basicval.objval = (uint64_t) (i + j + 4);

      if (!(i % 2)) {
        uarr_push(uarr_ptr, (void *) ((uintptr_t) (i + j)));
        uarr_push(uarr_int, i + j);
        uarr_push(uarr_obj, (uint64_t) (i + j));
        uarr_push(uarr_basic, basic);
        uarr_push(uarr_combine, combine);
      } else {
        void *iptr = (void *) ((uintptr_t) (i + j));
        uint64_t iobj = (uint64_t) (i + j);
        int ii = i + j;
        uarr_push_memcpy(uarr_ptr, &iptr);
        uarr_push_memcpy(uarr_int, &ii);
        uarr_push_memcpy(uarr_obj, &iobj);
        uarr_push_memcpy(uarr_basic, &basic);
        uarr_push_memcpy(uarr_combine, &combine);
      }
    }

    uarr_pop(uarr_ptr);
    uarr_pop(uarr_int);
    uarr_pop(uarr_obj);
    uarr_pop(uarr_basic);
    uarr_pop(uarr_combine);

    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_ptr));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_int));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_obj));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_basic));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_combine));

    for (j = 0; j <= i; ++j) {
      ASSERT_EQUAL((void *) ((uintptr_t) j), uarr_ptr[j]);
      ASSERT_EQUAL(j, uarr_int[j]);
      ASSERT_EQUAL((uint64_t) j, uarr_obj[j]);
      ASSERT_EQUAL(j, uarr_basic[j].intval);
      ASSERT_EQUAL((uint64_t) (j + 1), uarr_basic[j].objval);
      ASSERT_EQUAL(j, uarr_combine[j].basicval.intval);
      ASSERT_EQUAL((uint64_t) (j + 1), uarr_combine[j].basicval.objval);
      ASSERT_EQUAL(j + 2, uarr_combine[j].intval);
      ASSERT_EQUAL((char) j, uarr_combine[j].charval);
      ASSERT_EQUAL(0, uarr_combine[j].ptrval);
      ASSERT_EQUAL(j + 3, uarr_combine[j].unionval.basicval.intval);
      ASSERT_EQUAL((uint64_t) (j + 4), uarr_combine[j].unionval.basicval.objval);
    }
  }

  uarr_dtor(uarr_ptr);
  uarr_dtor(uarr_int);
  uarr_dtor(uarr_obj);
  uarr_dtor(uarr_basic);
  uarr_dtor(uarr_combine);
}

CTEST(array, inserterase) {
  int i, j, k;
  bool found;

  void **uarr_ptr = 0;
  int *uarr_int = 0;
  uint64_t *uarr_obj = 0;
  basic_t *uarr_basic = 0;
  combine_t *uarr_combine = 0;

  void **copy_ptr = 0;
  int *copy_int = 0;
  uint64_t *copy_obj = 0;
  basic_t *copy_basic = 0;
  combine_t *copy_combine = 0;

// Insert in empty
  {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    basic.intval = 1;
    basic.objval = 2;

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = 3;
    combine.basicval.objval = 4;
    combine.charval = 5;
    combine.intval = 6;
    combine.ptrval = 0;
    combine.unionval.realval = REAL_C(1.0);

    uarr_insert(uarr_ptr, 0, 0);
    uarr_insert(uarr_int, 0, 0);
    uarr_insert(uarr_obj, 0, 0);
    uarr_insert(uarr_basic, 0, basic);
    uarr_insert(uarr_combine, 0, combine);

    ASSERT_EQUAL(1, uarr_size(uarr_ptr));
    ASSERT_EQUAL(1, uarr_size(uarr_int));
    ASSERT_EQUAL(1, uarr_size(uarr_obj));
    ASSERT_EQUAL(1, uarr_size(uarr_basic));
    ASSERT_EQUAL(1, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_ptr[0]);
    ASSERT_EQUAL(0, uarr_int[0]);
    ASSERT_EQUAL(0, uarr_obj[0]);
    ASSERT_EQUAL(0, memcmp(uarr_basic, &basic, sizeof(basic)));
    ASSERT_EQUAL(0, memcmp(uarr_combine, &combine, sizeof(combine)));
  }
// Erase
  {
    int small_neg = -1;
    int large_neg = -1234;

    uarr_erase(uarr_ptr, 0);
    uarr_erase(uarr_int, 0);
    uarr_erase(uarr_obj, 0);
    uarr_erase(uarr_basic, 0);
    uarr_erase(uarr_combine, 0);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));

    uarr_erase_safe(uarr_ptr, small_neg);
    uarr_erase_safe(uarr_int, large_neg);
    uarr_erase_safe(uarr_obj, 0);
    uarr_erase_safe(uarr_basic, 1);
    uarr_erase_safe(uarr_combine, 1234);

    ASSERT_EQUAL(0, uarr_size(uarr_ptr));
    ASSERT_EQUAL(0, uarr_size(uarr_int));
    ASSERT_EQUAL(0, uarr_size(uarr_obj));
    ASSERT_EQUAL(0, uarr_size(uarr_basic));
    ASSERT_EQUAL(0, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));
  }
// Insert safe in empty
  {
    basic_t basic;
    combine_t combine;
    int small_neg = -1;
    int large_neg = -1234;

    memset(&basic, 0, sizeof(basic));
    basic.intval = 1;
    basic.objval = 2;

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = 3;
    combine.basicval.objval = 4;
    combine.charval = 5;
    combine.intval = 6;
    combine.ptrval = 0;
    combine.unionval.realval = REAL_C(1.0);

#define math_min(a, b) ((a)<(b)?(a):(b))

    uarr_insert_safe(uarr_ptr, large_neg, 0);
    uarr_insert_safe(uarr_int, small_neg, 0);
    uarr_insert_safe(uarr_obj, 0, 0);
    uarr_insert_safe(uarr_basic, 1, basic);
    uarr_insert_safe(uarr_combine, 1234, combine);

    ASSERT_EQUAL(1, uarr_size(uarr_ptr));
    ASSERT_EQUAL(1, uarr_size(uarr_int));
    ASSERT_EQUAL(1, uarr_size(uarr_obj));
    ASSERT_EQUAL(1, uarr_size(uarr_basic));
    ASSERT_EQUAL(1, uarr_size(uarr_combine));

    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_ptr));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_int));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_obj));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_basic));
    ASSERT_EQUAL(UARR_MIN_CAPACITY, uarr_capacity(uarr_combine));

    ASSERT_EQUAL(0, uarr_ptr[0]);
    ASSERT_EQUAL(0, uarr_int[0]);
    ASSERT_EQUAL(0, uarr_obj[0]);
    ASSERT_EQUAL(0, memcmp(uarr_basic, &basic, sizeof(basic)));
    ASSERT_EQUAL(0, memcmp(uarr_combine, &combine, sizeof(combine)));
  }

  uarr_dtor(uarr_ptr);
  uarr_dtor(uarr_int);
  uarr_dtor(uarr_obj);
  uarr_dtor(uarr_basic);
  uarr_dtor(uarr_combine);

// Insert sequence
  for (i = 0; i < 255; ++i) {
    basic_t basic;
    combine_t combine;

    memset(&basic, 0, sizeof(basic));
    basic.intval = i;
    basic.objval = (uint64_t) (i + 1);

    memset(&combine, 0, sizeof(combine));
    combine.basicval.intval = i + 2;
    combine.basicval.objval = (uint64_t) (i + 3);
    combine.charval = (char) (i + 4);
    combine.intval = i + 5;
    combine.ptrval = (void *) ((uintptr_t) (i + 6));
    combine.unionval.basicval.intval = i + 7;
    combine.unionval.basicval.objval = (uint64_t) (i + 8);

    uarr_insert_safe(uarr_ptr, 129 - i, (void *) ((uintptr_t) i));
    uarr_insert_safe(uarr_int, 129 - i, i);
    uarr_insert_safe(uarr_obj, 129 - i, (uint64_t) i);
    uarr_insert_safe(uarr_basic, 129 - i, basic);
    uarr_insert_safe(uarr_combine, 129 - i, combine);

    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_ptr));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_int));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_obj));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_basic));
    ASSERT_EQUAL((unsigned int) i + 1, uarr_size(uarr_combine));
  }

  uarr_clear(uarr_ptr);
  uarr_clear(uarr_int);
  uarr_clear(uarr_obj);
  uarr_clear(uarr_basic);
  uarr_clear(uarr_combine);

  uarr_dtor(copy_ptr);
  uarr_dtor(copy_int);
  uarr_dtor(copy_obj);
  uarr_dtor(copy_basic);
  uarr_dtor(copy_combine);

  uarr_dtor(uarr_ptr);
  uarr_dtor(uarr_int);
  uarr_dtor(uarr_obj);
  uarr_dtor(uarr_basic);
  uarr_dtor(uarr_combine);
}

CTEST(array, resize) {
  int *intarr = 0;
  int *prevarr = 0;

  uarr_resize(intarr, 139);

  ASSERT_NOT_EQUAL(intarr, 0);
  ASSERT_EQUAL(139, uarr_size(intarr));

  intarr[37] = 37;

  prevarr = intarr;
  uarr_resize(intarr, 139);

  ASSERT_EQUAL(prevarr, intarr);
  ASSERT_EQUAL(37, intarr[37]);
  ASSERT_EQUAL(139, uarr_size(intarr));

  uarr_resize(intarr, 38);

  ASSERT_NOT_EQUAL(intarr, 0);
  ASSERT_EQUAL(37, intarr[37]);
  ASSERT_EQUAL(38, uarr_size(intarr));

  uarr_reserve(intarr, 738);
  prevarr = intarr;
  uarr_resize(intarr, 738);
  uarr_resize(intarr, 738);

  ASSERT_EQUAL(prevarr, intarr);
  ASSERT_EQUAL(37, intarr[37]);
  ASSERT_EQUAL(738, uarr_size(intarr));

  uarr_resize(intarr, 0);

  ASSERT_NOT_EQUAL(intarr, 0);
  ASSERT_EQUAL(0, uarr_size(intarr));

  uarr_dtor(intarr);

  ASSERT_EQUAL(0, intarr);

  uarr_reserve(intarr, 15);
  ASSERT_EQUAL(16, uarr_capacity(intarr));
  ASSERT_EQUAL(0, uarr_size(intarr));

  uarr_grow(intarr, 2);
  ASSERT_EQUAL(16, uarr_capacity(intarr));
  ASSERT_EQUAL(2, uarr_size(intarr));

  uarr_grow(intarr, 15);
  ASSERT_EQUAL(16, uarr_capacity(intarr));
  ASSERT_EQUAL(17, uarr_size(intarr));

  uarr_dtor(intarr);

  ASSERT_EQUAL(0, intarr);
}

int main(int argc, const char *argv[]) {
  return ctest_main(argc, argv);
}
