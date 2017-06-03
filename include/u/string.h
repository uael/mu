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

/*!\file string.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_STRING_H__
# define U_STRING_H__

#include "types.h"

#ifdef __cplusplus
# include <cstring>
#else
# include <string.h>
#endif

#define SDS_MAX_PREALLOC (1024*1024)

typedef char *string_t;
typedef struct string_hdr5 string_hdr5_t;
typedef struct string_hdr8 string_hdr8_t;
typedef struct string_hdr16 string_hdr16_t;
typedef struct string_hdr32 string_hdr32_t;
typedef struct string_hdr64 string_hdr64_t;

/* Note: string_hdr5 is never used, we just access the flags byte directly.
 * However is here to document the layout of type 5 SDS strings. */
PACKED(struct string_hdr5 {
  uint8_t flags; /* 3 lsb of type, and 5 msb of string length */
  char buf[];
});
PACKED(struct string_hdr8 {
  uint8_t len; /* used */
  uint8_t alloc; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
});
PACKED(struct string_hdr16 {
  uint16_t len; /* used */
  uint16_t alloc; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
});
PACKED(struct string_hdr32 {
  uint32_t len; /* used */
  uint32_t alloc; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
});
PACKED(struct string_hdr64 {
  uint64_t len; /* used */
  uint64_t alloc; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
});

#define SDS_TYPE_5  0
#define SDS_TYPE_8  1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4
#define SDS_TYPE_MASK 7
#define SDS_TYPE_BITS 3
#define SDS_HDR_VAR(T, s) string_hdr##T##_t *sh = (void*)((s)-(sizeof(string_hdr##T##_t)));
#define SDS_HDR(T, s) ((string_hdr##T##_t *)((s)-(sizeof(string_hdr##T##_t))))
#define SDS_TYPE_5_LEN(f) ((f)>>SDS_TYPE_BITS)

static FORCEINLINE size_t string_len(const string_t s) {
  uint8_t flags = (uint8_t) s[-1];
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5:
      return SDS_TYPE_5_LEN(flags);
    case SDS_TYPE_8:
      return SDS_HDR(8, s)->len;
    case SDS_TYPE_16:
      return SDS_HDR(16, s)->len;
    case SDS_TYPE_32:
      return SDS_HDR(32, s)->len;
    case SDS_TYPE_64:
      return SDS_HDR(64, s)->len;
    default:
      break;
  }
  return 0;
}

static FORCEINLINE size_t string_avail(const string_t s) {
  uint8_t flags = (uint8_t) s[-1];
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5: {
      return 0;
    }
    case SDS_TYPE_8: {
      SDS_HDR_VAR(8, s);
      return sh->alloc - sh->len;
    }
    case SDS_TYPE_16: {
      SDS_HDR_VAR(16, s);
      return sh->alloc - sh->len;
    }
    case SDS_TYPE_32: {
      SDS_HDR_VAR(32, s);
      return sh->alloc - sh->len;
    }
    case SDS_TYPE_64: {
      SDS_HDR_VAR(64, s);
      return sh->alloc - sh->len;
    }
    default:
      break;
  }
  return 0;
}

static FORCEINLINE void string_setlen(string_t s, size_t newlen) {
  uint8_t flags = (uint8_t) s[-1];
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5: {
      uint8_t *fp = ((uint8_t *) s) - 1;
      *fp = (uint8_t) (SDS_TYPE_5 | (newlen << SDS_TYPE_BITS));
      break;
    }
    case SDS_TYPE_8:
      SDS_HDR(8, s)->len = (uint8_t) newlen;
      break;
    case SDS_TYPE_16:
      SDS_HDR(16, s)->len = (uint16_t) newlen;
      break;
    case SDS_TYPE_32:
      SDS_HDR(32, s)->len = (uint32_t) newlen;
      break;
    case SDS_TYPE_64:
      SDS_HDR(64, s)->len = newlen;
      break;
    default:
      break;
  }
}

static FORCEINLINE void string_inclen(string_t s, size_t inc) {
  uint8_t flags = (uint8_t) s[-1];
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5: {
      uint8_t *fp = ((uint8_t *) s) - 1;
      uint8_t newlen = (uint8_t) (SDS_TYPE_5_LEN(flags) + inc);
      *fp = (uint8_t) (SDS_TYPE_5 | (newlen << SDS_TYPE_BITS));
      break;
    }
    case SDS_TYPE_8:
      SDS_HDR(8, s)->len += inc;
      break;
    case SDS_TYPE_16:
      SDS_HDR(16, s)->len += inc;
      break;
    case SDS_TYPE_32:
      SDS_HDR(32, s)->len += inc;
      break;
    case SDS_TYPE_64:
      SDS_HDR(64, s)->len += inc;
      break;
    default:
      break;
  }
}

/* string_alloc() = string_avail() + string_len() */
static FORCEINLINE size_t string_alloc(const string_t s) {
  uint8_t flags = (uint8_t) s[-1];
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5:
      return SDS_TYPE_5_LEN(flags);
    case SDS_TYPE_8:
      return SDS_HDR(8, s)->alloc;
    case SDS_TYPE_16:
      return SDS_HDR(16, s)->alloc;
    case SDS_TYPE_32:
      return SDS_HDR(32, s)->alloc;
    case SDS_TYPE_64:
      return SDS_HDR(64, s)->alloc;
    default:
      break;
  }
  return 0;
}

static FORCEINLINE void string_setalloc(string_t s, size_t newlen) {
  uint8_t flags = (uint8_t) s[-1];
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5:
      /* Nothing to do, this type has no total allocation info. */
      break;
    case SDS_TYPE_8:
      SDS_HDR(8, s)->alloc = (uint8_t) newlen;
      break;
    case SDS_TYPE_16:
      SDS_HDR(16, s)->alloc = (uint16_t) newlen;
      break;
    case SDS_TYPE_32:
      SDS_HDR(32, s)->alloc = (uint32_t) newlen;
      break;
    case SDS_TYPE_64:
      SDS_HDR(64, s)->alloc = newlen;
      break;
    default:
      break;
  }
}

string_t string_newlen(const void *init, size_t initlen);
string_t string_new(const char *init);
string_t string_empty(void);
string_t string_dup(string_t s);
void string_free(string_t s);
string_t string_growzero(string_t s, size_t len);
string_t string_catlen(string_t s, const void *t, size_t len);
string_t string_cat(string_t s, const char *t);
string_t string_catsds(string_t s, string_t t);
string_t string_cpylen(string_t s, const char *t, size_t len);
string_t string_cpy(string_t s, const char *t);
string_t string_catvprintf(string_t s, const char *fmt, va_list ap);
string_t string_catprintf(string_t s, const char *fmt, ...) PRINTFCALL(2, 3);
string_t string_catfmt(string_t s, char const *fmt, ...);
string_t string_trim(string_t s, const char *cset);
void string_range(string_t s, int start, int end);
void string_updatelen(string_t s);
void string_clear(string_t s);
int string_cmp(string_t s1, string_t s2);
string_t *string_splitlen(const char *s, int len, const char *sep, int seplen, int *count);
void string_freesplitres(string_t *tokens, int count);
void string_tolower(string_t s);
void string_toupper(string_t s);
string_t string_fromlonglong(long long value);
string_t string_catrepr(string_t s, const char *p, size_t len);
string_t *string_splitargs(const char *line, int *argc);
string_t string_mapchars(string_t s, const char *from, const char *to, size_t setlen);
string_t string_join(char **argv, int argc, char *sep);
string_t string_joinsds(string_t *argv, int argc, const char *sep, size_t seplen);
string_t string_MakeRoomFor(string_t s, size_t addlen);
void string_IncrLen(string_t s, int incr);
string_t string_RemoveFreeSpace(string_t s);
size_t string_AllocSize(string_t s);
void *string_AllocPtr(string_t s);
void *string__malloc(size_t size);
void *string__realloc(void *ptr, size_t size);
void string__free(void *ptr);

#endif /* U_STRING_H__ */
