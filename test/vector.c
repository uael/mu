#include <u/vector.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

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

CTEST(vector, allocation) {
  uvec_of(void *) uvec_ptr = {0};
  uvec_of(int) uvec_int = {0};
  uvec_of(uint64_t) uvec_obj = {0};
  uvec_of(basic_t) uvec_basic = {0};
  uvec_of(combine_t) uvec_combine = {0};

  ASSERT_EQUAL(0, uvec_size(uvec_ptr));
  ASSERT_EQUAL(0, uvec_size(uvec_int));
  ASSERT_EQUAL(0, uvec_size(uvec_obj));
  ASSERT_EQUAL(0, uvec_size(uvec_basic));
  ASSERT_EQUAL(0, uvec_size(uvec_combine));

  ASSERT_EQUAL(0, uvec_capacity(uvec_ptr));
  ASSERT_EQUAL(0, uvec_capacity(uvec_int));
  ASSERT_EQUAL(0, uvec_capacity(uvec_obj));
  ASSERT_EQUAL(0, uvec_capacity(uvec_basic));
  ASSERT_EQUAL(0, uvec_capacity(uvec_combine));

  {
    uvec_reserve(uvec_ptr, 0);
    uvec_reserve(uvec_int, 0);
    uvec_reserve(uvec_obj, 0);
    uvec_reserve(uvec_basic, 0);
    uvec_reserve(uvec_combine, 0);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(0, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(0, uvec_capacity(uvec_int));
    ASSERT_EQUAL(0, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(0, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));

    uvec_reserve(uvec_ptr, 32);
    uvec_reserve(uvec_int, 33);
    uvec_reserve(uvec_obj, 63);
    uvec_reserve(uvec_basic, 1024);
    uvec_reserve(uvec_combine, 32742);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(32, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(64, uvec_capacity(uvec_int));
    ASSERT_EQUAL(64, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(1024, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(32768, uvec_capacity(uvec_combine));
  }
// Deallocate
  {
    ASSERT_EQUAL(nullptr, uvec_dtor(uvec_ptr));
    ASSERT_EQUAL(nullptr, uvec_dtor(uvec_int));
    ASSERT_EQUAL(nullptr, uvec_dtor(uvec_obj));
    ASSERT_EQUAL(nullptr, uvec_dtor(uvec_basic));
    ASSERT_EQUAL(nullptr, uvec_dtor(uvec_combine));

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(0, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(0, uvec_capacity(uvec_int));
    ASSERT_EQUAL(0, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(0, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));
  }
// Grow
  {
    uvec_grow(uvec_ptr, 75284);
    uvec_grow(uvec_int, 8295);
    uvec_grow(uvec_obj, 610);
    uvec_grow(uvec_basic, 24);
    uvec_grow(uvec_combine, 0);

    ASSERT_EQUAL(75284, uvec_size(uvec_ptr));
    ASSERT_EQUAL(8295, uvec_size(uvec_int));
    ASSERT_EQUAL(610, uvec_size(uvec_obj));
    ASSERT_EQUAL(24, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(131072, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(16384, uvec_capacity(uvec_int));
    ASSERT_EQUAL(1024, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(32, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));

    uvec_grow(uvec_ptr, 75284);
    uvec_grow(uvec_int, 8295);
    uvec_grow(uvec_obj, 610);
    uvec_grow(uvec_basic, 24);
    uvec_grow(uvec_combine, 0);

    ASSERT_EQUAL(75284 * 2, uvec_size(uvec_ptr));
    ASSERT_EQUAL(8295 * 2, uvec_size(uvec_int));
    ASSERT_EQUAL(610 * 2, uvec_size(uvec_obj));
    ASSERT_EQUAL(24 * 2, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(262144, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(32768, uvec_capacity(uvec_int));
    ASSERT_EQUAL(2048, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(64, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));

    uvec_grow(uvec_ptr, -1);
    uvec_grow(uvec_int, -2);
    uvec_grow(uvec_obj, -3);
    uvec_grow(uvec_basic, -4);
    uvec_grow(uvec_combine, 0);   //Grow is not bounds checked

    ASSERT_EQUAL(75284 * 2 - 1, uvec_size(uvec_ptr));
    ASSERT_EQUAL(8295 * 2 - 2, uvec_size(uvec_int));
    ASSERT_EQUAL(610 * 2 - 3, uvec_size(uvec_obj));
    ASSERT_EQUAL(24 * 2 - 4, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(262144, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(32768, uvec_capacity(uvec_int));
    ASSERT_EQUAL(2048, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(64, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));
  }
// Clear
  {
    uvec_clear(uvec_ptr);
    uvec_clear(uvec_int);
    uvec_clear(uvec_obj);
    uvec_clear(uvec_basic);
    uvec_clear(uvec_combine);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(262144, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(32768, uvec_capacity(uvec_int));
    ASSERT_EQUAL(2048, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(64, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));

    uvec_dtor(uvec_ptr);
    uvec_dtor(uvec_int);
    uvec_dtor(uvec_obj);
    uvec_dtor(uvec_basic);
    uvec_dtor(uvec_combine);

    uvec_clear(uvec_ptr);
    uvec_clear(uvec_int);
    uvec_clear(uvec_obj);
    uvec_clear(uvec_basic);
    uvec_clear(uvec_combine);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(0, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(0, uvec_capacity(uvec_int));
    ASSERT_EQUAL(0, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(0, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(0, uvec_capacity(uvec_combine));
  }
// Reserve + grow
  {
    uvec_reserve(uvec_ptr, 1);
    uvec_reserve(uvec_int, 2);
    uvec_reserve(uvec_obj, 3);
    uvec_reserve(uvec_basic, 4);
    uvec_reserve(uvec_combine, 5);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(8, uvec_capacity(uvec_combine));

    uvec_grow(uvec_ptr, 1);
    uvec_grow(uvec_int, 2);
    uvec_grow(uvec_obj, 3);
    uvec_grow(uvec_basic, 4);
    uvec_grow(uvec_combine, 5);

    ASSERT_EQUAL(1, uvec_size(uvec_ptr));
    ASSERT_EQUAL(2, uvec_size(uvec_int));
    ASSERT_EQUAL(3, uvec_size(uvec_obj));
    ASSERT_EQUAL(4, uvec_size(uvec_basic));
    ASSERT_EQUAL(5, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(8, uvec_capacity(uvec_combine));

    uvec_grow(uvec_ptr, 1);
    uvec_grow(uvec_int, 2);
    uvec_grow(uvec_obj, 3);
    uvec_grow(uvec_basic, 4);
    uvec_grow(uvec_combine, 5);

    ASSERT_EQUAL(1 * 2, uvec_size(uvec_ptr));
    ASSERT_EQUAL(2 * 2, uvec_size(uvec_int));
    ASSERT_EQUAL(3 * 2, uvec_size(uvec_obj));
    ASSERT_EQUAL(4 * 2, uvec_size(uvec_basic));
    ASSERT_EQUAL(5 * 2, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(8, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(8, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(16, uvec_capacity(uvec_combine));
  }

  uvec_dtor(uvec_ptr);
  uvec_dtor(uvec_int);
  uvec_dtor(uvec_obj);
  uvec_dtor(uvec_basic);
  uvec_dtor(uvec_combine);
}

CTEST(vector, copy) {
  int i;
  uvec_of(void *) uvec_ptr = {0};
  uvec_of(int) uvec_int = {0};
  uvec_of(uint64_t) uvec_obj = {0};
  uvec_of(basic_t) uvec_basic = {0};
  uvec_of(combine_t) uvec_combine = {0};

  uvec_of(void *) copy_ptr = {0};
  uvec_of(int) copy_int = {0};
  uvec_of(uint64_t) copy_obj = {0};
  uvec_of(basic_t) copy_basic = {0};
  uvec_of(combine_t) copy_combine = {0};

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

    uvec_push(uvec_ptr, (void *) ((uintptr_t) i));
    uvec_push(uvec_int, i);
    uvec_push(uvec_obj, (uint64_t) i);
    uvec_push(uvec_basic, basic);
    uvec_push(uvec_combine, combine);
  }

  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_copy(copy_ptr, uvec_ptr));
  ASSERT_EQUAL(uvec_size(uvec_int), uvec_copy(copy_int, uvec_int));
  ASSERT_EQUAL(uvec_size(uvec_obj), uvec_copy(copy_obj, uvec_obj));
  ASSERT_EQUAL(uvec_size(uvec_basic), uvec_copy(copy_basic, uvec_basic));
  ASSERT_EQUAL(uvec_size(uvec_combine), uvec_copy(copy_combine, uvec_combine));

  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_ptr));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_int));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_obj));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_basic));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uvec_ptr.data[i], copy_ptr.data[i]);
    ASSERT_EQUAL(uvec_int.data[i], copy_int.data[i]);
    ASSERT_EQUAL(uvec_obj.data[i], copy_obj.data[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic.data + i, uvec_basic.data + i, sizeof(*copy_basic.data)));
    ASSERT_EQUAL(0, memcmp(copy_combine.data + i, uvec_combine.data + i, sizeof(*copy_combine.data)));
  }

  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_copy(copy_ptr, uvec_ptr));
  ASSERT_EQUAL(uvec_size(uvec_int), uvec_copy(copy_int, uvec_int));
  ASSERT_EQUAL(uvec_size(uvec_obj), uvec_copy(copy_obj, uvec_obj));
  ASSERT_EQUAL(uvec_size(uvec_basic), uvec_copy(copy_basic, uvec_basic));
  ASSERT_EQUAL(uvec_size(uvec_combine), uvec_copy(copy_combine, uvec_combine));

  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_ptr));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_int));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_obj));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_basic));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uvec_ptr.data[i], copy_ptr.data[i]);
    ASSERT_EQUAL(uvec_int.data[i], copy_int.data[i]);
    ASSERT_EQUAL(uvec_obj.data[i], copy_obj.data[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic.data + i, uvec_basic.data + i, sizeof(*copy_basic.data)));
    ASSERT_EQUAL(0, memcmp(copy_combine.data + i, uvec_combine.data + i, sizeof(*copy_combine.data)));
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

    uvec_push(copy_ptr, (void *) ((uintptr_t) i));
    uvec_push(copy_int, i);
    uvec_push(copy_obj, (uint64_t) i);
    uvec_push(copy_basic, basic);
    uvec_push(copy_combine, combine);
  }

  uvec_erase(copy_ptr, 0);
  uvec_erase(copy_int, 0);
  uvec_erase(copy_obj, 0);
  uvec_erase(copy_basic, 0);
  uvec_erase(copy_combine, 0);

  uvec_copy(copy_ptr, uvec_ptr);
  uvec_copy(copy_int, uvec_int);
  uvec_copy(copy_obj, uvec_obj);
  uvec_copy(copy_basic, uvec_basic);
  uvec_copy(copy_combine, uvec_combine);

  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_ptr));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_int));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_obj));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_basic));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uvec_ptr.data[i], copy_ptr.data[i]);
    ASSERT_EQUAL(uvec_int.data[i], copy_int.data[i]);
    ASSERT_EQUAL(uvec_obj.data[i], copy_obj.data[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic.data + i, uvec_basic.data + i, sizeof(*copy_basic.data)));
    ASSERT_EQUAL(0, memcmp(copy_combine.data + i, uvec_combine.data + i, sizeof(*copy_combine.data)));
  }

  uvec_clear(copy_ptr);
  uvec_clear(copy_int);
  uvec_clear(copy_obj);
  uvec_clear(copy_basic);
  uvec_clear(copy_combine);

  uvec_copy(copy_ptr, uvec_ptr);
  uvec_copy(copy_int, uvec_int);
  uvec_copy(copy_obj, uvec_obj);
  uvec_copy(copy_basic, uvec_basic);
  uvec_copy(copy_combine, uvec_combine);

  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_ptr));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_int));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_obj));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_basic));
  ASSERT_EQUAL(uvec_size(uvec_ptr), uvec_size(copy_combine));

  for (i = 0; i < 255; ++i) {
    ASSERT_EQUAL(uvec_ptr.data[i], copy_ptr.data[i]);
    ASSERT_EQUAL(uvec_int.data[i], copy_int.data[i]);
    ASSERT_EQUAL(uvec_obj.data[i], copy_obj.data[i]);
    ASSERT_EQUAL(0, memcmp(copy_basic.data + i, uvec_basic.data + i, sizeof(*copy_basic.data)));
    ASSERT_EQUAL(0, memcmp(copy_combine.data + i, uvec_combine.data + i, sizeof(*copy_combine.data)));
  }

  uvec_dtor(uvec_ptr);
  uvec_dtor(uvec_int);
  uvec_dtor(uvec_obj);
  uvec_dtor(uvec_basic);
  uvec_dtor(uvec_combine);

  uvec_dtor(copy_ptr);
  uvec_dtor(copy_int);
  uvec_dtor(copy_obj);
  uvec_dtor(copy_basic);
  uvec_dtor(copy_combine);
}

CTEST(vector, pushpop) {
  int i, j;
  uvec_of(void *) uvec_ptr = {0};
  uvec_of(int) uvec_int = {0};
  uvec_of(uint64_t) uvec_obj = {0};
  uvec_of(basic_t) uvec_basic = {0};
  uvec_of(combine_t) uvec_combine = {0};

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

    ASSERT_EQUAL(0, uvec_push(uvec_ptr, 0));
    ASSERT_EQUAL(0, uvec_push(uvec_int, 0));
    ASSERT_EQUAL(0, uvec_push(uvec_obj, 0));
    uvec_push(uvec_basic, basic);
    uvec_push(uvec_combine, combine);

    ASSERT_EQUAL(1, uvec_size(uvec_ptr));
    ASSERT_EQUAL(1, uvec_size(uvec_int));
    ASSERT_EQUAL(1, uvec_size(uvec_obj));
    ASSERT_EQUAL(1, uvec_size(uvec_basic));
    ASSERT_EQUAL(1, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));

    ASSERT_EQUAL(0, uvec_ptr.data[0]);
    ASSERT_EQUAL(0, uvec_int.data[0]);
    ASSERT_EQUAL(0, uvec_obj.data[0]);
    ASSERT_EQUAL(0, memcmp(uvec_basic.data, &basic, sizeof(basic)));
    ASSERT_EQUAL(0, memcmp(uvec_combine.data, &combine, sizeof(combine)));
  }
// Pop
  {
    uvec_pop(uvec_ptr);
    uvec_pop(uvec_int);
    uvec_pop(uvec_obj);
    uvec_pop(uvec_basic);
    uvec_pop(uvec_combine);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));

    uvec_pop_safe(uvec_ptr);
    uvec_pop_safe(uvec_int);
    uvec_pop_safe(uvec_obj);
    uvec_pop_safe(uvec_basic);
    uvec_pop_safe(uvec_combine);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));
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
      uvec_push(uvec_ptr, (void *) ((uintptr_t) i));
      uvec_push(uvec_int, i);
      uvec_push(uvec_obj, (uint64_t) i);
      uvec_push(uvec_basic, basic);
      uvec_push(uvec_combine, combine);
    } else {
      void *iptr = (void *) ((uintptr_t) i);
      uint64_t iobj = (uint64_t) i;
      uvec_push_memcpy(uvec_ptr, &iptr);
      uvec_push_memcpy(uvec_int, &i);
      uvec_push_memcpy(uvec_obj, &iobj);
      uvec_push_memcpy(uvec_basic, &basic);
      uvec_push_memcpy(uvec_combine, &combine);
    }

    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_ptr));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_int));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_obj));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_basic));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_combine));

    for (j = 0; j <= i; ++j) {
      ASSERT_EQUAL((void *) ((uintptr_t) j), uvec_ptr.data[j]);
      ASSERT_EQUAL(j, uvec_int.data[j]);
      ASSERT_EQUAL((uint64_t) j, uvec_obj.data[j]);
      ASSERT_EQUAL(j, uvec_basic.data[j].intval);
      ASSERT_EQUAL((uint64_t) j + 1, uvec_basic.data[j].objval);
      ASSERT_EQUAL(j, uvec_combine.data[j].basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 1, uvec_combine.data[j].basicval.objval);
      ASSERT_EQUAL(j + 2, uvec_combine.data[j].intval);
      ASSERT_EQUAL((char) j, uvec_combine.data[j].charval);
      ASSERT_EQUAL(0, uvec_combine.data[j].ptrval);
      ASSERT_EQUAL(j + 3, uvec_combine.data[j].unionval.basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 4, uvec_combine.data[j].unionval.basicval.objval);
    }
  }
// Pop while non-empty
  for (; i > 0; --i) {
    uvec_pop(uvec_ptr);
    uvec_pop(uvec_int);
    uvec_pop(uvec_obj);
    uvec_pop(uvec_basic);
    uvec_pop(uvec_combine);

    ASSERT_EQUAL((unsigned int) i - 1, uvec_size(uvec_ptr));
    ASSERT_EQUAL((unsigned int) i - 1, uvec_size(uvec_int));
    ASSERT_EQUAL((unsigned int) i - 1, uvec_size(uvec_obj));
    ASSERT_EQUAL((unsigned int) i - 1, uvec_size(uvec_basic));
    ASSERT_EQUAL((unsigned int) i - 1, uvec_size(uvec_combine));

    for (j = 0; j < i; ++j) {
      ASSERT_EQUAL((void *) ((uintptr_t) j), uvec_ptr.data[j]);
      ASSERT_EQUAL(j, uvec_int.data[j]);
      ASSERT_EQUAL((uint64_t) j, uvec_obj.data[j]);
      ASSERT_EQUAL(j, uvec_basic.data[j].intval);
      ASSERT_EQUAL((uint64_t) j + 1, uvec_basic.data[j].objval);
      ASSERT_EQUAL(j, uvec_combine.data[j].basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 1, uvec_combine.data[j].basicval.objval);
      ASSERT_EQUAL(j + 2, uvec_combine.data[j].intval);
      ASSERT_EQUAL((char) j, uvec_combine.data[j].charval);
      ASSERT_EQUAL(0, uvec_combine.data[j].ptrval);
      ASSERT_EQUAL(j + 3, uvec_combine.data[j].unionval.basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 4, uvec_combine.data[j].unionval.basicval.objval);
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
        uvec_push(uvec_ptr, (void *) ((uintptr_t) (i + j)));
        uvec_push(uvec_int, i + j);
        uvec_push(uvec_obj, (uint64_t) (i + j));
        uvec_push(uvec_basic, basic);
        uvec_push(uvec_combine, combine);
      } else {
        void *iptr = (void *) ((uintptr_t) (i + j));
        uint64_t iobj = (uint64_t) (i + j);
        int ii = i + j;
        uvec_push_memcpy(uvec_ptr, &iptr);
        uvec_push_memcpy(uvec_int, &ii);
        uvec_push_memcpy(uvec_obj, &iobj);
        uvec_push_memcpy(uvec_basic, &basic);
        uvec_push_memcpy(uvec_combine, &combine);
      }
    }

    uvec_pop(uvec_ptr);
    uvec_pop(uvec_int);
    uvec_pop(uvec_obj);
    uvec_pop(uvec_basic);
    uvec_pop(uvec_combine);

    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_ptr));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_int));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_obj));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_basic));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_combine));

    for (j = 0; j <= i; ++j) {
      ASSERT_EQUAL((void *) ((uintptr_t) j), uvec_ptr.data[j]);
      ASSERT_EQUAL(j, uvec_int.data[j]);
      ASSERT_EQUAL((uint64_t) j, uvec_obj.data[j]);
      ASSERT_EQUAL(j, uvec_basic.data[j].intval);
      ASSERT_EQUAL((uint64_t) j + 1, uvec_basic.data[j].objval);
      ASSERT_EQUAL(j, uvec_combine.data[j].basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 1, uvec_combine.data[j].basicval.objval);
      ASSERT_EQUAL(j + 2, uvec_combine.data[j].intval);
      ASSERT_EQUAL((char) j, uvec_combine.data[j].charval);
      ASSERT_EQUAL(0, uvec_combine.data[j].ptrval);
      ASSERT_EQUAL(j + 3, uvec_combine.data[j].unionval.basicval.intval);
      ASSERT_EQUAL((uint64_t) j + 4, uvec_combine.data[j].unionval.basicval.objval);
    }
  }

  uvec_dtor(uvec_ptr);
  uvec_dtor(uvec_int);
  uvec_dtor(uvec_obj);
  uvec_dtor(uvec_basic);
  uvec_dtor(uvec_combine);
}

CTEST(vector, inserterase) {
  int i, j, k;
  bool found;

  uvec_of(void *) uvec_ptr = {0};
  uvec_of(int) uvec_int = {0};
  uvec_of(uint64_t) uvec_obj = {0};
  uvec_of(basic_t) uvec_basic = {0};
  uvec_of(combine_t) uvec_combine = {0};

  uvec_of(void *) copy_ptr = {0};
  uvec_of(int) copy_int = {0};
  uvec_of(uint64_t) copy_obj = {0};
  uvec_of(basic_t) copy_basic = {0};
  uvec_of(combine_t) copy_combine = {0};

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

    uvec_insert(uvec_ptr, 0, 0);
    uvec_insert(uvec_int, 0, 0);
    uvec_insert(uvec_obj, 0, 0);
    uvec_insert(uvec_basic, 0, basic);
    uvec_insert(uvec_combine, 0, combine);

    ASSERT_EQUAL(1, uvec_size(uvec_ptr));
    ASSERT_EQUAL(1, uvec_size(uvec_int));
    ASSERT_EQUAL(1, uvec_size(uvec_obj));
    ASSERT_EQUAL(1, uvec_size(uvec_basic));
    ASSERT_EQUAL(1, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));

    ASSERT_EQUAL(0, uvec_ptr.data[0]);
    ASSERT_EQUAL(0, uvec_int.data[0]);
    ASSERT_EQUAL(0, uvec_obj.data[0]);
    ASSERT_EQUAL(0, memcmp(uvec_basic.data, &basic, sizeof(basic)));
    ASSERT_EQUAL(0, memcmp(uvec_combine.data, &combine, sizeof(combine)));
  }
// Erase
  {
    int small_neg = -1;
    int large_neg = -1234;

    uvec_erase(uvec_ptr, 0);
    uvec_erase(uvec_int, 0);
    uvec_erase(uvec_obj, 0);
    uvec_erase(uvec_basic, 0);
    uvec_erase(uvec_combine, 0);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));

    uvec_erase_safe(uvec_ptr, small_neg);
    uvec_erase_safe(uvec_int, large_neg);
    uvec_erase_safe(uvec_obj, 0);
    uvec_erase_safe(uvec_basic, 1);
    uvec_erase_safe(uvec_combine, 1234);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));
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

    uvec_insert_safe(uvec_ptr, large_neg, 0);
    uvec_insert_safe(uvec_int, small_neg, 0);
    uvec_insert_safe(uvec_obj, 0, 0);
    uvec_insert_safe(uvec_basic, 1, basic);
    uvec_insert_safe(uvec_combine, 1234, combine);

    ASSERT_EQUAL(0, uvec_size(uvec_ptr));
    ASSERT_EQUAL(0, uvec_size(uvec_int));
    ASSERT_EQUAL(0, uvec_size(uvec_obj));
    ASSERT_EQUAL(0, uvec_size(uvec_basic));
    ASSERT_EQUAL(0, uvec_size(uvec_combine));

    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_ptr));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_int));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_obj));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_basic));
    ASSERT_EQUAL(UVEC_MIN_CAPACITY, uvec_capacity(uvec_combine));

    ASSERT_EQUAL(0, uvec_ptr.data[0]);
    ASSERT_EQUAL(0, uvec_int.data[0]);
    ASSERT_EQUAL(0, uvec_obj.data[0]);
    ASSERT_EQUAL(0, memcmp(uvec_basic.data, &basic, sizeof(basic)));
    ASSERT_EQUAL(0, memcmp(uvec_combine.data, &combine, sizeof(combine)));
  }

  uvec_dtor(uvec_ptr);
  uvec_dtor(uvec_int);
  uvec_dtor(uvec_obj);
  uvec_dtor(uvec_basic);
  uvec_dtor(uvec_combine);

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

    uvec_insert(uvec_ptr, 129 - i, (void *) ((uintptr_t) i));
    uvec_insert(uvec_int, 129 - i, i);
    uvec_insert(uvec_obj, 129 - i, (uint64_t) i);
    uvec_insert(uvec_basic, 129 - i, basic);
    uvec_insert(uvec_combine, 129 - i, combine);

    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_ptr));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_int));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_obj));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_basic));
    ASSERT_EQUAL((unsigned int) i + 1, uvec_size(uvec_combine));
  }

  uvec_clear(uvec_ptr);
  uvec_clear(uvec_int);
  uvec_clear(uvec_obj);
  uvec_clear(uvec_basic);
  uvec_clear(uvec_combine);

  uvec_dtor(copy_ptr);
  uvec_dtor(copy_int);
  uvec_dtor(copy_obj);
  uvec_dtor(copy_basic);
  uvec_dtor(copy_combine);

  uvec_dtor(uvec_ptr);
  uvec_dtor(uvec_int);
  uvec_dtor(uvec_obj);
  uvec_dtor(uvec_basic);
  uvec_dtor(uvec_combine);
}

CTEST(vector, resize) {
  uvec_of(int) intarr = {0};
  uvec_of(int) prevarr = {0};

  uvec_resize(intarr, 139);

  ASSERT_NOT_EQUAL(uvec_data(intarr), 0);
  ASSERT_EQUAL(139, uvec_size(intarr));

  uvec_data(intarr)[37] = 37;

  prevarr.data = intarr.data;
  prevarr.size = intarr.size;
  prevarr.capacity = intarr.capacity;
  uvec_resize(intarr, 139);

  ASSERT_EQUAL(prevarr.data, intarr.data);
  ASSERT_EQUAL(37, intarr.data[37]);
  ASSERT_EQUAL(139, uvec_size(intarr));

  uvec_resize(intarr, 38);

  ASSERT_NOT_EQUAL(intarr.data, 0);
  ASSERT_EQUAL(37, intarr.data[37]);
  ASSERT_EQUAL(38, uvec_size(intarr));

  uvec_reserve(intarr, 738);
  prevarr.data = intarr.data;
  prevarr.size = intarr.size;
  prevarr.capacity = intarr.capacity;
  uvec_resize(intarr, 738);
  uvec_resize(intarr, 738);

  ASSERT_EQUAL(prevarr.data, intarr.data);
  ASSERT_EQUAL(37, intarr.data[37]);
  ASSERT_EQUAL(738, uvec_size(intarr));

  uvec_resize(intarr, 0);

  ASSERT_NOT_EQUAL(intarr.data, 0);
  ASSERT_EQUAL(0, uvec_size(intarr));

  uvec_dtor(intarr);

  ASSERT_EQUAL(0, intarr.data);

  uvec_reserve(intarr, 15);
  ASSERT_EQUAL(16, uvec_capacity(intarr));
  ASSERT_EQUAL(0, uvec_size(intarr));

  uvec_grow(intarr, 2);
  ASSERT_EQUAL(16, uvec_capacity(intarr));
  ASSERT_EQUAL(2, uvec_size(intarr));

  uvec_grow(intarr, 15);
  ASSERT_EQUAL(16, uvec_capacity(intarr));
  ASSERT_EQUAL(17, uvec_size(intarr));

  uvec_dtor(intarr);

  ASSERT_EQUAL(0, intarr.data);
}

int main(int argc, const char *argv[]) {
  return ctest_main(argc, argv);
}
