#include <time.h>

#include <gtest/gtest.h>

#include <u/vector.h>
#include <u/math.h>

#define VECTOR_DUMP(n) "vector[size='" << ds_size(v ## n) << "', capacity='" << ds_cap(v ## n) << "']"
#define ASSERT_CAPACITY(_q0, _q1, _q2, _q3, _q4) \
  ASSERT_EQ(_q0, ds_cap(v0)) << _q0 << " => " << VECTOR_DUMP(0); \
  ASSERT_EQ(_q1, ds_cap(v1)) << _q1 << " => " << VECTOR_DUMP(1); \
  ASSERT_EQ(_q2, ds_cap(v2)) << _q2 << " => " << VECTOR_DUMP(2); \
  ASSERT_EQ(_q3, ds_cap(v3)) << _q3 << " => " << VECTOR_DUMP(3); \
  ASSERT_EQ(_q4, ds_cap(v4)) << _q4 << " => " << VECTOR_DUMP(4)
#define ASSERT_SIZE(_s0, _s1, _s2, _s3, _s4) \
  ASSERT_EQ(_s0, ds_size(v0)) << _s0 << " => " << VECTOR_DUMP(0); \
  ASSERT_EQ(_s1, ds_size(v1)) << _s1 << " => " << VECTOR_DUMP(1); \
  ASSERT_EQ(_s2, ds_size(v2)) << _s2 << " => " << VECTOR_DUMP(2); \
  ASSERT_EQ(_s3, ds_size(v3)) << _s3 << " => " << VECTOR_DUMP(3); \
  ASSERT_EQ(_s4, ds_size(v4)) << _s4 << " => " << VECTOR_DUMP(4)


struct VectorTest : public ::testing::Test {
  virtual void SetUp() {
    std::random_device rd;
    std::mt19937 eng(rd());

    std::uniform_int_distribution<> dist0(0, 10);
    std::uniform_int_distribution<> dist1(10, 100);
    std::uniform_int_distribution<> dist2(100, 1000);
    std::uniform_int_distribution<> dist3(1000, 10000);
    std::uniform_int_distribution<> dist4(10000, 100000);

    s0 = (size_t) dist0(eng);
    s1 = (size_t) dist1(eng);
    s2 = (size_t) dist2(eng);
    s3 = (size_t) dist3(eng);
    s4 = (size_t) dist4(eng);

    qRoundUp32();
  }

  virtual void TearDown() {
    vec_dtor(v0);
    vec_dtor(v1);
    vec_dtor(v2);
    vec_dtor(v3);
    vec_dtor(v4);
  }

  void qRoundUp32() {
    q0 = roundup32(s0);
    q1 = roundup32(s1);
    q2 = roundup32(s2);
    q3 = roundup32(s3);
    q4 = roundup32(s4);

    if (q0 && q0 < UVEC_MIN_CAPACITY) q0 = UVEC_MIN_CAPACITY;
  }

  struct Point {
    long long x, y;
  };

  size_t s0, q0;
  size_t s1, q1;
  size_t s2, q2;
  size_t s3, q3;
  size_t s4, q4;

  vec_of(int) v0 = {0};
  vec_of(size_t) v1 = {0};
  vec_of(struct { int x; int y; }) v2 = {0};
  vec_of(Point) v3 = {0};
  vec_of(void *) v4 = {0};
};

TEST_F(VectorTest, grow) {
  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_grow(v0, s0);
  vec_grow(v1, s1);
  vec_grow(v2, s2);
  vec_grow(v3, s3);
  vec_grow(v4, s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  s0 *= 2;
  s1 *= 2;
  s2 *= 2;
  s3 *= 2;
  s4 *= 2;

  vec_grow(v0, s0);
  vec_grow(v1, s1);
  vec_grow(v2, s2);
  vec_grow(v3, s3);
  vec_grow(v4, s4);

  qRoundUp32();

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_grow(v0, -(s0/2));
  vec_grow(v1, -(s1/2));
  vec_grow(v2, -(s2/2));
  vec_grow(v3, -(s3/2));
  vec_grow(v4, -(s4/2));

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_grow(v0, -s0);
  vec_grow(v1, -s1);
  vec_grow(v2, -s2);
  vec_grow(v3, -s3);
  vec_grow(v4, -s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_grow(v0, 1);
  vec_grow(v1, 1);
  vec_grow(v2, 1);
  vec_grow(v3, 1);
  vec_grow(v4, 1);

  ASSERT_CAPACITY((q0 ? q0 : UVEC_MIN_CAPACITY), q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);
}

TEST_F(VectorTest, reserve) {
  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_reserve(v0, s0);
  vec_reserve(v1, s1);
  vec_reserve(v2, s2);
  vec_reserve(v3, s3);
  vec_reserve(v4, s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  s0 *= 6;
  s1 *= 5;
  s2 *= 4;
  s3 *= 3;
  s4 *= 2;

  qRoundUp32();

  vec_reserve(v0, s0);
  vec_reserve(v1, s1);
  vec_reserve(v2, s2);
  vec_reserve(v3, s3);
  vec_reserve(v4, s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_reserve(v0, -s0);
  vec_reserve(v1, -s1);
  vec_reserve(v2, -s2);
  vec_reserve(v3, -s3);
  vec_reserve(v4, -s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);
}

TEST_F(VectorTest, resize) {
  ASSERT_CAPACITY(0, 0, 0, 0, 0);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_resize(v0, s0);
  vec_resize(v1, s1);
  vec_resize(v2, s2);
  vec_resize(v3, s3);
  vec_resize(v4, s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(s0, s1, s2, s3, s4);

  vec_resize(v0, -s0);
  vec_resize(v1, -s1);
  vec_resize(v2, -s2);
  vec_resize(v3, -s3);
  vec_resize(v4, -s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_resize(v0, -s0);
  vec_resize(v1, -s1);
  vec_resize(v2, -s2);
  vec_resize(v3, -s3);
  vec_resize(v4, -s4);

  ASSERT_CAPACITY(q0, q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);

  vec_resize(v0, 1);
  vec_resize(v1, 1);
  vec_resize(v2, 1);
  vec_resize(v3, 1);
  vec_resize(v4, 1);

  ASSERT_CAPACITY((q0 ? q0 : UVEC_MIN_CAPACITY), q1, q2, q3, q4);
  ASSERT_SIZE(1, 1, 1, 1, 1);

  vec_resize(v0, 0);
  vec_resize(v1, 0);
  vec_resize(v2, 0);
  vec_resize(v3, 0);
  vec_resize(v4, 0);

  ASSERT_CAPACITY((q0 ? q0 : UVEC_MIN_CAPACITY), q1, q2, q3, q4);
  ASSERT_SIZE(0, 0, 0, 0, 0);
}

TEST_F(VectorTest, push) {

}
