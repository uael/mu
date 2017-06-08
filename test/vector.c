#include <time.h>
#include "cute.h"

#include "u/math.h"
#include "u/vector.h"

typedef struct point point_t;

struct point {
  long long x, y;
};

typedef uvec_of(int) v0_t;
typedef uvec_of(size_t) v1_t;
typedef uvec_of(struct { int x; int y; }) v2_t;
typedef uvec_of(point_t) v3_t;
typedef uvec_of(void *) v4_t;

CUTEST_DATA {
  v0_t v0;
  v1_t v1;
  v2_t v2;
  v3_t v3;
  v4_t v4;

  size_t s0, q0;
  size_t s1, q1;
  size_t s2, q2;
  size_t s3, q3;
  size_t s4, q4;
};

static void test_qroudup32(CUTEST_DATA *self) {
  self->q0 = roundup32(self->s0);
  self->q1 = roundup32(self->s1);
  self->q2 = roundup32(self->s2);
  self->q3 = roundup32(self->s3);
  self->q4 = roundup32(self->s4);

  if (self->q0 && self->q0 < UVEC_MIN_CAPACITY) self->q0 = UVEC_MIN_CAPACITY;
}

CUTEST_SETUP {
  srand((unsigned) time(NULL));

  self->s0 = (size_t) (rand() % 10);
  self->s1 = (size_t) (rand() % (100 - 10) + 10);
  self->s2 = (size_t) (rand() % (1000 - 100) + 100);
  self->s3 = (size_t) (rand() % (10000 - 1000) + 1000);
  self->s4 = (size_t) (rand() % (100000 - 10000) + 10000);

  test_qroudup32(self);

  self->v0 = (v0_t) {0};
  self->v1 = (v1_t) {0};
  self->v2 = (v2_t) {0};
  self->v3 = (v3_t) {0};
  self->v4 = (v4_t) {0};
}

CUTEST_TEARDOWN {
  uvec_dtor(self->v0);
  uvec_dtor(self->v1);
  uvec_dtor(self->v2);
  uvec_dtor(self->v3);
  uvec_dtor(self->v4);
}

CUTEST(vector, grow);
CUTEST(vector, reserve);
CUTEST(vector, resize);
CUTEST(vector, push);

int main(void) {
  CUTEST_DATA test = {0};

  CUTEST_PASS(vector, grow);
  CUTEST_PASS(vector, reserve);
  CUTEST_PASS(vector, resize);
  CUTEST_PASS(vector, push);

  return EXIT_SUCCESS;
}

#define ASSERT_CAPACITY(_q0, _q1, _q2, _q3, _q4) do { \
    ASSERT(_q0 == ds_cap(self->v0)); \
    ASSERT(_q1 == ds_cap(self->v1)); \
    ASSERT(_q2 == ds_cap(self->v2)); \
    ASSERT(_q3 == ds_cap(self->v3)); \
    ASSERT(_q4 == ds_cap(self->v4)); \
  } while (false)

#define ASSERT_SIZE(_s0, _s1, _s2, _s3, _s4) do { \
    ASSERT(_s0 == ds_size(self->v0)); \
    ASSERT(_s1 == ds_size(self->v1)); \
    ASSERT(_s2 == ds_size(self->v2)); \
    ASSERT(_s3 == ds_size(self->v3)); \
    ASSERT(_s4 == ds_size(self->v4)); \
  } while (false)

CUTEST(vector, grow) {
  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_grow(self->v0, self->s0);
  uvec_grow(self->v1, self->s1);
  uvec_grow(self->v2, self->s2);
  uvec_grow(self->v3, self->s3);
  uvec_grow(self->v4, self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  self->s0 *= 2;
  self->s1 *= 2;
  self->s2 *= 2;
  self->s3 *= 2;
  self->s4 *= 2;

  uvec_grow(self->v0, self->s0);
  uvec_grow(self->v1, self->s1);
  uvec_grow(self->v2, self->s2);
  uvec_grow(self->v3, self->s3);
  uvec_grow(self->v4, self->s4);

  test_qroudup32(self);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_grow(self->v0, -(self->s0/2));
  uvec_grow(self->v1, -(self->s1/2));
  uvec_grow(self->v2, -(self->s2/2));
  uvec_grow(self->v3, -(self->s3/2));
  uvec_grow(self->v4, -(self->s4/2));

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_grow(self->v0, -self->s0);
  uvec_grow(self->v1, -self->s1);
  uvec_grow(self->v2, -self->s2);
  uvec_grow(self->v3, -self->s3);
  uvec_grow(self->v4, -self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_grow(self->v0, 1);
  uvec_grow(self->v1, 1);
  uvec_grow(self->v2, 1);
  uvec_grow(self->v3, 1);
  uvec_grow(self->v4, 1);

  ASSERT_CAPACITY((self->q0 ? self->q0 : UVEC_MIN_CAPACITY), self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  return NULL;
}

CUTEST(vector, reserve) {
  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_growth(self->v0, self->s0);
  uvec_growth(self->v1, self->s1);
  uvec_growth(self->v2, self->s2);
  uvec_growth(self->v3, self->s3);
  uvec_growth(self->v4, self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  self->s0 *= 6;
  self->s1 *= 5;
  self->s2 *= 4;
  self->s3 *= 3;
  self->s4 *= 2;

  test_qroudup32(self);

  uvec_growth(self->v0, self->s0);
  uvec_growth(self->v1, self->s1);
  uvec_growth(self->v2, self->s2);
  uvec_growth(self->v3, self->s3);
  uvec_growth(self->v4, self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_growth(self->v0, -self->s0);
  uvec_growth(self->v1, -self->s1);
  uvec_growth(self->v2, -self->s2);
  uvec_growth(self->v3, -self->s3);
  uvec_growth(self->v4, -self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  return NULL;
}

CUTEST(vector, resize) {
  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_resize(self->v0, self->s0);
  uvec_resize(self->v1, self->s1);
  uvec_resize(self->v2, self->s2);
  uvec_resize(self->v3, self->s3);
  uvec_resize(self->v4, self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(self->s0, self->s1, self->s2, self->s3, self->s4);

  uvec_resize(self->v0, -self->s0);
  uvec_resize(self->v1, -self->s1);
  uvec_resize(self->v2, -self->s2);
  uvec_resize(self->v3, -self->s3);
  uvec_resize(self->v4, -self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_resize(self->v0, -self->s0);
  uvec_resize(self->v1, -self->s1);
  uvec_resize(self->v2, -self->s2);
  uvec_resize(self->v3, -self->s3);
  uvec_resize(self->v4, -self->s4);

  ASSERT_CAPACITY(self->q0, self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  uvec_resize(self->v0, 1);
  uvec_resize(self->v1, 1);
  uvec_resize(self->v2, 1);
  uvec_resize(self->v3, 1);
  uvec_resize(self->v4, 1);

  ASSERT_CAPACITY((self->q0 ? self->q0 : UVEC_MIN_CAPACITY), self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(1, 1, 1, 1, 1);

  uvec_resize(self->v0, 0);
  uvec_resize(self->v1, 0);
  uvec_resize(self->v2, 0);
  uvec_resize(self->v3, 0);
  uvec_resize(self->v4, 0);

  ASSERT_CAPACITY((self->q0 ? self->q0 : UVEC_MIN_CAPACITY), self->q1, self->q2, self->q3, self->q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  return NULL;
}

CUTEST(vector, push) {
  int i, i0;

  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  for (i = 0; i<100; ++i) {
    uvec_push(self->v0, i);
  }

  foreach(i0, in(T_uvec, self->v0)) {
    ASSERT(i0 == uvec_it(self->v0) - uvec_begin(self->v0));
    ASSERT(i0 == *uvec_it(self->v0));
  }

  ASSERT(uvec_it(self->v0) == uvec_end(self->v0));
  ASSERT(100 == uvec_it(self->v0) - uvec_begin(self->v0));

  rforeach(i0, in(T_uvec, self->v0)) {
    ASSERT(i0 == uvec_it(self->v0) - uvec_begin(self->v0));
    ASSERT(i0 == *uvec_it(self->v0));
  }

  ASSERT(uvec_it(self->v0) == uvec_begin(self->v0)-1);
  ASSERT(-1 == uvec_it(self->v0) - uvec_begin(self->v0));

  return CUTE_SUCCESS;
}
