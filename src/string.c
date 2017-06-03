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

#include <stdio.h>
#include <assert.h>
#include "u/string.h"

static FORCEINLINE int string_HdrSize(char type) {
  switch (type & SDS_TYPE_MASK) {
    case SDS_TYPE_5:
      return sizeof(string_hdr5_t);
    case SDS_TYPE_8:
      return sizeof(string_hdr8_t);
    case SDS_TYPE_16:
      return sizeof(string_hdr16_t);
    case SDS_TYPE_32:
      return sizeof(string_hdr32_t);
    case SDS_TYPE_64:
      return sizeof(string_hdr64_t);
    default:
      break;
  }
  return 0;
}

static FORCEINLINE char string_ReqType(size_t string_size) {
  if (string_size < 1 << 5)
    return SDS_TYPE_5;
  if (string_size < 1 << 8)
    return SDS_TYPE_8;
  if (string_size < 1 << 16)
    return SDS_TYPE_16;
#if (LONG_MAX == LLONG_MAX)
  if (string_size < 1ll << 32)
    return SDS_TYPE_32;
#endif
  return SDS_TYPE_64;
}

/* Create a new string_t string with the content specified by the 'init' pointer
 * and 'initlen'.
 * If nullptr is used for 'init' the string is initialized with zero bytes.
 *
 * The string is always null-termined (all the string_t strings are, always) so
 * even if you create an string_t string with:
 *
 * mystring = string_newlen("abc",3);
 *
 * You can print the string with printf() as there is an implicit \0 at the
 * end of the string. However the string is binary safe and can contain
 * \0 characters in the middle, as the length is stored in the string_t header. */
string_t string_newlen(const void *init, size_t initlen) {
  void *sh;
  string_t s;
  char type = string_ReqType(initlen);
  /* Empty strings are usually created in order to append. Use type 8
   * since type 5 is not good at this. */
  if (type == SDS_TYPE_5 && initlen == 0)
    type = SDS_TYPE_8;
  int hdrlen = string_HdrSize(type);
  uint8_t *fp; /* flags pointer. */

  sh = malloc(hdrlen + initlen + 1);
  if (!init)
    memset(sh, 0, hdrlen + initlen + 1);
  if (sh == nullptr)
    return nullptr;
  s = (char *) sh + hdrlen;
  fp = ((uint8_t *) s) - 1;
  switch (type) {
    case SDS_TYPE_5: {
      *fp = (uint8_t) (type | (initlen << SDS_TYPE_BITS));
      break;
    }
    case SDS_TYPE_8: {
      SDS_HDR_VAR(8, s);
      sh->len = (uint8_t) initlen;
      sh->alloc = (uint8_t) initlen;
      *fp = (uint8_t) type;
      break;
    }
    case SDS_TYPE_16: {
      SDS_HDR_VAR(16, s);
      sh->len = (uint16_t) initlen;
      sh->alloc = (uint16_t) initlen;
      *fp = (uint8_t) type;
      break;
    }
    case SDS_TYPE_32: {
      SDS_HDR_VAR(32, s);
      sh->len = (uint32_t) initlen;
      sh->alloc = (uint32_t) initlen;
      *fp = (uint8_t) type;
      break;
    }
    case SDS_TYPE_64: {
      SDS_HDR_VAR(64, s);
      sh->len = initlen;
      sh->alloc = initlen;
      *fp = (uint8_t) type;
      break;
    }
    default:
      break;
  }
  if (initlen && init)
    memcpy(s, init, initlen);
  s[initlen] = '\0';
  return s;
}

/* Create an empty (zero length) string_t string. Even in this case the string
 * always has an implicit null term. */
string_t string_empty(void) {
  return string_newlen("", 0);
}

/* Create a new string_t string starting from a null terminated C string. */
string_t string_new(const char *init) {
  size_t initlen = (init == nullptr) ? 0 : strlen(init);
  return string_newlen(init, initlen);
}

/* Duplicate an string_t string. */
string_t string_dup(const string_t s) {
  return string_newlen(s, string_len(s));
}

/* Free an string_t string. No operation is performed if 's' is nullptr. */
void string_free(string_t s) {
  if (s == nullptr)
    return;
  free((char *) s - string_HdrSize(s[-1]));
}

/* Set the string_t string length to the length as obtained with strlen(), so
 * considering as content only up to the first null term character.
 *
 * This function is useful when the string_t string is hacked manually in some
 * way, like in the following example:
 *
 * s = string_new("foobar");
 * s[2] = '\0';
 * string_updatelen(s);
 * printf("%d\n", string_len(s));
 *
 * The output will be "2", but if we comment out the call to string_updatelen()
 * the output will be "6" as the string was modified but the logical length
 * remains 6 bytes. */
void string_updatelen(string_t s) {
  int reallen = (int) strlen(s);
  string_setlen(s, (size_t) reallen);
}

/* Modify an string_t string in-place to make it empty (zero length).
 * However all the existing buffer is not discarded but set as free space
 * so that next append operations will not require allocations up to the
 * number of bytes previously available. */
void string_clear(string_t s) {
  string_setlen(s, 0);
  s[0] = '\0';
}

/* Enlarge the free space at the end of the string_t string so that the caller
 * is sure that after calling this function can overwrite up to addlen
 * bytes after the end of the string, plus one more byte for nul term.
 *
 * Note: this does not change the *length* of the string_t string as returned
 * by string_len(), but only the free buffer space we have. */
string_t string_MakeRoomFor(string_t s, size_t addlen) {
  void *sh, *newsh;
  size_t avail = string_avail(s);
  size_t len, newlen;
  char type, oldtype = (char) (s[-1] & SDS_TYPE_MASK);
  int hdrlen;

  /* Return ASAP if there is enough space left. */
  if (avail >= addlen)
    return s;

  len = string_len(s);
  sh = (char *) s - string_HdrSize(oldtype);
  newlen = (len + addlen);
  if (newlen < SDS_MAX_PREALLOC)
    newlen *= 2;
  else
    newlen += SDS_MAX_PREALLOC;

  type = string_ReqType(newlen);

  /* Don't use type 5: the user is appending to the string and type 5 is
   * not able to remember empty space, so string_MakeRoomFor() must be called
   * at every appending operation. */
  if (type == SDS_TYPE_5)
    type = SDS_TYPE_8;

  hdrlen = string_HdrSize(type);
  if (oldtype == type) {
    newsh = realloc(sh, hdrlen + newlen + 1);
    if (newsh == nullptr)
      return nullptr;
    s = (char *) newsh + hdrlen;
  } else {
    /* Since the header size changes, need to move the string forward,
     * and can't use realloc */
    newsh = malloc(hdrlen + newlen + 1);
    if (newsh == nullptr)
      return nullptr;
    memcpy((char *) newsh + hdrlen, s, len + 1);
    free(sh);
    s = (char *) newsh + hdrlen;
    s[-1] = type;
    string_setlen(s, len);
  }
  string_setalloc(s, newlen);
  return s;
}

/* Reallocate the string_t string so that it has no free space at the end. The
 * contained string remains not altered, but next concatenation operations
 * will require a reallocation.
 *
 * After the call, the passed string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
string_t string_RemoveFreeSpace(string_t s) {
  void *sh, *newsh;
  char type, oldtype = (char) (s[-1] & SDS_TYPE_MASK);
  int hdrlen;
  size_t len = string_len(s);
  sh = (char *) s - string_HdrSize(oldtype);

  type = string_ReqType(len);
  hdrlen = string_HdrSize(type);
  if (oldtype == type) {
    newsh = realloc(sh, hdrlen + len + 1);
    if (newsh == nullptr)
      return nullptr;
    s = (char *) newsh + hdrlen;
  } else {
    newsh = malloc(hdrlen + len + 1);
    if (newsh == nullptr)
      return nullptr;
    memcpy((char *) newsh + hdrlen, s, len + 1);
    free(sh);
    s = (char *) newsh + hdrlen;
    s[-1] = type;
    string_setlen(s, len);
  }
  string_setalloc(s, len);
  return s;
}

/* Return the total size of the allocation of the specifed string_t string,
 * including:
 * 1) The string_t header before the pointer.
 * 2) The string.
 * 3) The free buffer at the end if any.
 * 4) The implicit null term.
 */
size_t string_AllocSize(string_t s) {
  size_t alloc = string_alloc(s);
  return string_HdrSize(s[-1]) + alloc + 1;
}

/* Return the pointer of the actual SDS allocation (normally SDS strings
 * are referenced by the start of the string buffer). */
void *string_AllocPtr(string_t s) {
  return (void *) (s - string_HdrSize(s[-1]));
}

/* Increment the string_t length and decrements the left free space at the
 * end of the string according to 'incr'. Also set the null term
 * in the new end of the string.
 *
 * This function is used in order to fix the string length after the
 * user calls string_MakeRoomFor(), writes something after the end of
 * the current string, and finally needs to set the new length.
 *
 * Note: it is possible to use a negative increment in order to
 * right-trim the string.
 *
 * Usage example:
 *
 * Using string_IncrLen() and string_MakeRoomFor() it is possible to mount the
 * following schema, to cat bytes coming from the kernel to the end of an
 * string_t string without copying into an intermediate buffer:
 *
 * oldlen = string_len(s);
 * s = string_MakeRoomFor(s, BUFFER_SIZE);
 * nread = read(fd, s+oldlen, BUFFER_SIZE);
 * ... check for nread <= 0 and handle it ...
 * string_IncrLen(s, nread);
 */
void string_IncrLen(string_t s, int incr) {
  uint8_t flags = (uint8_t) s[-1];
  size_t len;
  switch (flags & SDS_TYPE_MASK) {
    case SDS_TYPE_5: {
      uint8_t *fp = ((uint8_t *) s) - 1;
      uint8_t oldlen = SDS_TYPE_5_LEN(flags);
      assert((incr > 0 && oldlen + incr < 32) || (incr < 0 && oldlen >= (unsigned int) (-incr)));
      *fp = (uint8_t) (SDS_TYPE_5 | ((oldlen + incr) << SDS_TYPE_BITS));
      len = (size_t) (oldlen + incr);
      break;
    }
    case SDS_TYPE_8: {
      SDS_HDR_VAR(8, s);
      assert((incr >= 0 && sh->alloc - sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int) (-incr)));
      len = (sh->len += incr);
      break;
    }
    case SDS_TYPE_16: {
      SDS_HDR_VAR(16, s);
      assert((incr >= 0 && sh->alloc - sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int) (-incr)));
      len = (sh->len += incr);
      break;
    }
    case SDS_TYPE_32: {
      SDS_HDR_VAR(32, s);
      assert(
        (incr >= 0 && sh->alloc - sh->len >= (unsigned int) incr) || (incr < 0 && sh->len >= (unsigned int) (-incr)));
      len = (sh->len += incr);
      break;
    }
    case SDS_TYPE_64: {
      SDS_HDR_VAR(64, s);
      assert((incr >= 0 && sh->alloc - sh->len >= (uint64_t) incr) || (incr < 0 && sh->len >= (uint64_t) (-incr)));
      len = (sh->len += incr);
      break;
    }
    default:
      len = 0; /* Just to avoid compilation warnings. */
  }
  s[len] = '\0';
}

/* Grow the string_t to have the specified length. Bytes that were not part of
 * the original length of the string_t will be set to zero.
 *
 * if the specified length is smaller than the current length, no operation
 * is performed. */
string_t string_growzero(string_t s, size_t len) {
  size_t curlen = string_len(s);

  if (len <= curlen)
    return s;
  s = string_MakeRoomFor(s, len - curlen);
  if (s == nullptr)
    return nullptr;

  /* Make sure added region doesn't contain garbage */
  memset(s + curlen, 0, (len - curlen + 1)); /* also set trailing \0 byte */
  string_setlen(s, len);
  return s;
}

/* Append the specified binary-safe string pointed by 't' of 'len' bytes to the
 * end of the specified string_t string 's'.
 *
 * After the call, the passed string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
string_t string_catlen(string_t s, const void *t, size_t len) {
  size_t curlen = string_len(s);

  s = string_MakeRoomFor(s, len);
  if (s == nullptr)
    return nullptr;
  memcpy(s + curlen, t, len);
  string_setlen(s, curlen + len);
  s[curlen + len] = '\0';
  return s;
}

/* Append the specified null termianted C string to the string_t string 's'.
 *
 * After the call, the passed string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
string_t string_cat(string_t s, const char *t) {
  return string_catlen(s, t, strlen(t));
}

/* Append the specified string_t 't' to the existing string_t 's'.
 *
 * After the call, the modified string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
string_t string_catsds(string_t s, const string_t t) {
  return string_catlen(s, t, string_len(t));
}

/* Destructively modify the string_t string 's' to hold the specified binary
 * safe string pointed by 't' of length 'len' bytes. */
string_t string_cpylen(string_t s, const char *t, size_t len) {
  if (string_alloc(s) < len) {
    s = string_MakeRoomFor(s, len - string_len(s));
    if (s == nullptr)
      return nullptr;
  }
  memcpy(s, t, len);
  s[len] = '\0';
  string_setlen(s, len);
  return s;
}

/* Like string_cpylen() but 't' must be a null-termined string so that the length
 * of the string is obtained with strlen(). */
string_t string_cpy(string_t s, const char *t) {
  return string_cpylen(s, t, strlen(t));
}

/* Helper for string_catlonglong() doing the actual number -> string
 * conversion. 's' must point to a string with room for at least
 * SDS_LLSTR_SIZE bytes.
 *
 * The function returns the length of the null-terminated string
 * representation stored at 's'. */
#define SDS_LLSTR_SIZE 21
static int string_ll2str(char *s, long long value) {
  char *p, aux;
  unsigned long long v;
  size_t l;

  /* Generate the string representation, this method produces
   * an reversed string. */
  v = (unsigned long long) ((value < 0) ? -value : value);
  p = s;
  do {
    *p++ = (char) ('0' + (v % 10));
    v /= 10;
  } while (v);
  if (value < 0)
    *p++ = '-';

  /* Compute length and add null term. */
  l = p - s;
  *p = '\0';

  /* Reverse the string. */
  p--;
  while (s < p) {
    aux = *s;
    *s = *p;
    *p = aux;
    s++;
    p--;
  }
  return (int) l;
}

/* Identical string_ll2str(), but for unsigned long long type. */
static int string_ull2str(char *s, unsigned long long v) {
  char *p, aux;
  size_t l;

  /* Generate the string representation, this method produces
   * an reversed string. */
  p = s;
  do {
    *p++ = (char) ('0' + (v % 10));
    v /= 10;
  } while (v);

  /* Compute length and add null term. */
  l = p - s;
  *p = '\0';

  /* Reverse the string. */
  p--;
  while (s < p) {
    aux = *s;
    *s = *p;
    *p = aux;
    s++;
    p--;
  }
  return (int) l;
}

/* Create an string_t string from a long long value. It is much faster than:
 *
 * string_catprintf(string_empty(),"%lld\n", value);
 */
string_t string_fromlonglong(long long value) {
  char buf[SDS_LLSTR_SIZE];
  int len = string_ll2str(buf, value);

  return string_newlen(buf, (size_t) len);
}

/* Like string_catprintf() but gets va_list instead of being variadic. */
string_t string_catvprintf(string_t s, const char *fmt, va_list ap) {
  va_list cpy;
  char staticbuf[1024], *buf = staticbuf, *t;
  size_t buflen = strlen(fmt) * 2;

  /* We try to start using a static buffer for speed.
   * If not possible we revert to heap allocation. */
  if (buflen > sizeof(staticbuf)) {
    buf = malloc(buflen);
    if (buf == nullptr)
      return nullptr;
  } else {
    buflen = sizeof(staticbuf);
  }

  /* Try with buffers two times bigger every time we fail to
   * fit the string in the current buffer size. */
  while (1) {
    buf[buflen - 2] = '\0';
    va_copy(cpy, ap);
    vsnprintf(buf, buflen, fmt, cpy);
    va_end(cpy);
    if (buf[buflen - 2] != '\0') {
      if (buf != staticbuf)
        free(buf);
      buflen *= 2;
      buf = malloc(buflen);
      if (buf == nullptr)
        return nullptr;
      continue;
    }
    break;
  }

  /* Finally concat the obtained string to the SDS string and return it. */
  t = string_cat(s, buf);
  if (buf != staticbuf)
    free(buf);
  return t;
}

/* Append to the string_t string 's' a string obtained using printf-alike format
 * specifier.
 *
 * After the call, the modified string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call.
 *
 * Example:
 *
 * s = string_new("Sum is: ");
 * s = string_catprintf(s,"%d+%d = %d",a,b,a+b).
 *
 * Often you need to create a string from scratch with the printf-alike
 * format. When this is the need, just use string_empty() as the target string:
 *
 * s = string_catprintf(string_empty(), "... your format ...", args);
 */
string_t string_catprintf(string_t s, const char *fmt, ...) {
  va_list ap;
  char *t;
  va_start(ap, fmt);
  t = string_catvprintf(s, fmt, ap);
  va_end(ap);
  return t;
}

/* This function is similar to string_catprintf, but much faster as it does
 * not rely on sprintf() family functions implemented by the libc that
 * are often very slow. Moreover directly handling the string_t string as
 * new data is concatenated provides a performance improvement.
 *
 * However this function only handles an incompatible subset of printf-alike
 * format specifiers:
 *
 * %s - C String
 * %S - SDS string
 * %i - signed int
 * %I - 64 bit signed integer (long long, int64_t)
 * %u - unsigned int
 * %U - 64 bit unsigned integer (unsigned long long, uint64_t)
 * %% - Verbatim "%" character.
 */
string_t string_catfmt(string_t s, char const *fmt, ...) {
  size_t initlen = string_len(s);
  const char *f;
  int i;
  va_list ap;

  va_start(ap, fmt);
  f = fmt;    /* Next format specifier byte to process. */
  i = (int) initlen; /* Position of the next byte to write to dest str. */
  while (*f) {
    char next, *str;
    size_t l;
    long long num;
    unsigned long long unum;

    /* Make sure there is always space for at least 1 char. */
    if (string_avail(s) == 0) {
      s = string_MakeRoomFor(s, 1);
    }

    switch (*f) {
      case '%':
        next = *(f + 1);
        f++;
        switch (next) {
          case 's':
          case 'S':
            str = va_arg(ap, char*);
            l = (next == 's') ? strlen(str) : string_len(str);
            if (string_avail(s) < l) {
              s = string_MakeRoomFor(s, l);
            }
            memcpy(s + i, str, l);
            string_inclen(s, l);
            i += l;
            break;
          case 'i':
          case 'I':
            if (next == 'i')
              num = va_arg(ap, int);
            else
              num = va_arg(ap, long long);
            {
              char buf[SDS_LLSTR_SIZE];
              l = (size_t) string_ll2str(buf, num);
              if (string_avail(s) < l) {
                s = string_MakeRoomFor(s, l);
              }
              memcpy(s + i, buf, l);
              string_inclen(s, l);
              i += l;
            }
            break;
          case 'u':
          case 'U':
            if (next == 'u')
              unum = va_arg(ap, unsigned int);
            else
              unum = va_arg(ap, unsigned long long);
            {
              char buf[SDS_LLSTR_SIZE];
              l = (size_t) string_ull2str(buf, unum);
              if (string_avail(s) < l) {
                s = string_MakeRoomFor(s, l);
              }
              memcpy(s + i, buf, l);
              string_inclen(s, l);
              i += l;
            }
            break;
          default: /* Handle %% and generally %<unknown>. */
            s[i++] = next;
            string_inclen(s, 1);
            break;
        }
        break;
      default:
        s[i++] = *f;
        string_inclen(s, 1);
        break;
    }
    f++;
  }
  va_end(ap);

  /* Add null-term */
  s[i] = '\0';
  return s;
}

/* Remove the part of the string from left and from right composed just of
 * contiguous characters found in 'cset', that is a null terminted C string.
 *
 * After the call, the modified string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call.
 *
 * Example:
 *
 * s = string_new("AA...AA.a.aa.aHelloWorld     :::");
 * s = string_trim(s,"Aa. :");
 * printf("%s\n", s);
 *
 * Output will be just "Hello World".
 */
string_t string_trim(string_t s, const char *cset) {
  char *start, *end, *sp, *ep;
  size_t len;

  sp = start = s;
  ep = end = s + string_len(s) - 1;
  while (sp <= end && strchr(cset, *sp))
    sp++;
  while (ep > sp && strchr(cset, *ep))
    ep--;
  len = (sp > ep) ? 0 : ((ep - sp) + 1);
  if (s != sp)
    memmove(s, sp, len);
  s[len] = '\0';
  string_setlen(s, len);
  return s;
}

/* Turn the string into a smaller (or equal) string containing only the
 * substring specified by the 'start' and 'end' indexes.
 *
 * start and end can be negative, where -1 means the last character of the
 * string, -2 the penultimate character, and so forth.
 *
 * The interval is inclusive, so the start and end characters will be part
 * of the resulting string.
 *
 * The string is modified in-place.
 *
 * Example:
 *
 * s = string_new("Hello World");
 * string_range(s,1,-1); => "ello World"
 */
void string_range(string_t s, int start, int end) {
  size_t newlen, len = string_len(s);

  if (len == 0)
    return;
  if (start < 0) {
    start = (int) (len + start);
    if (start < 0)
      start = 0;
  }
  if (end < 0) {
    end = (int) (len + end);
    if (end < 0)
      end = 0;
  }
  newlen = (size_t) ((start > end) ? 0 : (end - start) + 1);
  if (newlen != 0) {
    if (start >= (signed) len) {
      newlen = 0;
    } else if (end >= (signed) len) {
      end = (int) (len - 1);
      newlen = (size_t) ((start > end) ? 0 : (end - start) + 1);
    }
  } else {
    start = 0;
  }
  if (start && newlen)
    memmove(s, s + start, newlen);
  s[newlen] = 0;
  string_setlen(s, newlen);
}

/* Apply tolower() to every character of the string_t string 's'. */
void string_tolower(string_t s) {
  size_t len = string_len(s), j;

  for (j = 0; j < len; j++)
    s[j] = (char) tolower(s[j]);
}

/* Apply toupper() to every character of the string_t string 's'. */
void string_toupper(string_t s) {
  size_t len = string_len(s), j;

  for (j = 0; j < len; j++)
    s[j] = (char) toupper(s[j]);
}

/* Compare two string_t strings s1 and s2 with memcmp().
 *
 * Return value:
 *
 *     positive if s1 > s2.
 *     negative if s1 < s2.
 *     0 if s1 and s2 are exactly the same binary string.
 *
 * If two strings share exactly the same prefix, but one of the two has
 * additional characters, the longer string is considered to be greater than
 * the smaller one. */
int string_cmp(const string_t s1, const string_t s2) {
  size_t l1, l2, minlen;
  int cmp;

  l1 = string_len(s1);
  l2 = string_len(s2);
  minlen = (l1 < l2) ? l1 : l2;
  cmp = memcmp(s1, s2, minlen);
  if (cmp == 0)
    return (int) (l1 - l2);
  return cmp;
}

/* Split 's' with separator in 'sep'. An array
 * of string_t strings is returned. *count will be set
 * by reference to the number of tokens returned.
 *
 * On out of memory, zero length string, zero length
 * separator, nullptr is returned.
 *
 * Note that 'sep' is able to split a string using
 * a multi-character separator. For example
 * string_split("foo_-_bar","_-_"); will return two
 * elements "foo" and "bar".
 *
 * This version of the function is binary-safe but
 * requires length arguments. string_split() is just the
 * same function but for zero-terminated strings.
 */
string_t *string_splitlen(const char *s, int len, const char *sep, int seplen, int *count) {
  int elements = 0, slots = 5, start = 0, j;
  string_t *tokens;

  if (seplen < 1 || len < 0)
    return nullptr;

  tokens = malloc(sizeof(string_t) * slots);
  if (tokens == nullptr)
    return nullptr;

  if (len == 0) {
    *count = 0;
    return tokens;
  }
  for (j = 0; j < (len - (seplen - 1)); j++) {
    /* make sure there is room for the next element and the final one */
    if (slots < elements + 2) {
      string_t *newtokens;

      slots *= 2;
      newtokens = realloc(tokens, sizeof(string_t) * slots);
      if (newtokens == nullptr)
        goto cleanup;
      tokens = newtokens;
    }
    /* search the separator */
    if ((seplen == 1 && *(s + j) == sep[0]) || (memcmp(s + j, sep, (size_t) seplen) == 0)) {
      tokens[elements] = string_newlen(s + start, (size_t) (j - start));
      if (tokens[elements] == nullptr)
        goto cleanup;
      elements++;
      start = j + seplen;
      j = j + seplen - 1; /* skip the separator */
    }
  }
  /* Add the final element. We are sure there is room in the tokens array. */
  tokens[elements] = string_newlen(s + start, (size_t) (len - start));
  if (tokens[elements] == nullptr)
    goto cleanup;
  elements++;
  *count = elements;
  return tokens;

    cleanup:
  {
    int i;
    for (i = 0; i < elements; i++)
      string_free(tokens[i]);
    free(tokens);
    *count = 0;
    return nullptr;
  }
}

/* Free the result returned by string_splitlen(), or do nothing if 'tokens' is nullptr. */
void string_freesplitres(string_t *tokens, int count) {
  if (!tokens)
    return;
  while (count--)
    string_free(tokens[count]);
  free(tokens);
}

/* Append to the string_t string "s" an escaped string representation where
 * all the non-printable characters (tested with isprint()) are turned into
 * escapes in the form "\n\r\a...." or "\x<hex-number>".
 *
 * After the call, the modified string_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
string_t string_catrepr(string_t s, const char *p, size_t len) {
  s = string_catlen(s, "\"", 1);
  while (len--) {
    switch (*p) {
      case '\\':
      case '"':
        s = string_catprintf(s, "\\%c", *p);
        break;
      case '\n':
        s = string_catlen(s, "\\n", 2);
        break;
      case '\r':
        s = string_catlen(s, "\\r", 2);
        break;
      case '\t':
        s = string_catlen(s, "\\t", 2);
        break;
      case '\a':
        s = string_catlen(s, "\\a", 2);
        break;
      case '\b':
        s = string_catlen(s, "\\b", 2);
        break;
      default:
        if (isprint(*p))
          s = string_catprintf(s, "%c", *p);
        else
          s = string_catprintf(s, "\\x%02x", (uint8_t) *p);
        break;
    }
    p++;
  }
  return string_catlen(s, "\"", 1);
}

/* Helper function for string_splitargs() that returns non zero if 'c'
 * is a valid hex digit. */
static int is_hex_digit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
    (c >= 'A' && c <= 'F');
}

/* Helper function for string_splitargs() that converts a hex digit into an
 * integer from 0 to 15 */
static int hex_digit_to_int(char c) {
  switch (c) {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
    case 'A':
      return 10;
    case 'b':
    case 'B':
      return 11;
    case 'c':
    case 'C':
      return 12;
    case 'd':
    case 'D':
      return 13;
    case 'e':
    case 'E':
      return 14;
    case 'f':
    case 'F':
      return 15;
    default:
      return 0;
  }
}

/* Split a line into arguments, where every argument can be in the
 * following programming-language REPL-alike form:
 *
 * foo bar "newline are supported\n" and "\xff\x00otherstuff"
 *
 * The number of arguments is stored into *argc, and an array
 * of string_t is returned.
 *
 * The caller should free the resulting array of string_t strings with
 * string_freesplitres().
 *
 * Note that string_catrepr() is able to convert back a string into
 * a quoted string in the same format string_splitargs() is able to parse.
 *
 * The function returns the allocated tokens on success, even when the
 * input string is empty, or nullptr if the input contains unbalanced
 * quotes or closed quotes followed by non space characters
 * as in: "foo"bar or "foo'
 */
string_t *string_splitargs(const char *line, int *argc) {
  const char *p = line;
  char *current = nullptr;
  char **vector = nullptr;

  *argc = 0;
  while (1) {
    /* skip blanks */
    while (*p && isspace(*p))
      p++;
    if (*p) {
      /* get a token */
      int inq = 0;  /* set to 1 if we are in "quotes" */
      int insq = 0; /* set to 1 if we are in 'single quotes' */
      int done = 0;

      if (current == nullptr)
        current = string_empty();
      while (!done) {
        if (inq) {
          if (*p == '\\' && *(p + 1) == 'x' &&
            is_hex_digit(*(p + 2)) &&
            is_hex_digit(*(p + 3))) {
            uint8_t byte;

            byte = (uint8_t) ((hex_digit_to_int(*(p + 2)) * 16) +
              hex_digit_to_int(*(p + 3)));
            current = string_catlen(current, (char *) &byte, 1);
            p += 3;
          } else if (*p == '\\' && *(p + 1)) {
            char c;

            p++;
            switch (*p) {
              case 'n':
                c = '\n';
                break;
              case 'r':
                c = '\r';
                break;
              case 't':
                c = '\t';
                break;
              case 'b':
                c = '\b';
                break;
              case 'a':
                c = '\a';
                break;
              default:
                c = *p;
                break;
            }
            current = string_catlen(current, &c, 1);
          } else if (*p == '"') {
            /* closing quote must be followed by a space or
             * nothing at all. */
            if (*(p + 1) && !isspace(*(p + 1)))
              goto err;
            done = 1;
          } else if (!*p) {
            /* unterminated quotes */
            goto err;
          } else {
            current = string_catlen(current, p, 1);
          }
        } else if (insq) {
          if (*p == '\\' && *(p + 1) == '\'') {
            p++;
            current = string_catlen(current, "'", 1);
          } else if (*p == '\'') {
            /* closing quote must be followed by a space or
             * nothing at all. */
            if (*(p + 1) && !isspace(*(p + 1)))
              goto err;
            done = 1;
          } else if (!*p) {
            /* unterminated quotes */
            goto err;
          } else {
            current = string_catlen(current, p, 1);
          }
        } else {
          switch (*p) {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
            case '\0':
              done = 1;
              break;
            case '"':
              inq = 1;
              break;
            case '\'':
              insq = 1;
              break;
            default:
              current = string_catlen(current, p, 1);
              break;
          }
        }
        if (*p)
          p++;
      }
      /* add the token to the vector */
      vector = realloc(vector, ((*argc) + 1) * sizeof(char *));
      vector[*argc] = current;
      (*argc)++;
      current = nullptr;
    } else {
      /* Even on empty input string return something not nullptr. */
      if (vector == nullptr)
        vector = malloc(sizeof(void *));
      return vector;
    }
  }

    err:
  while ((*argc)--)
    string_free(vector[*argc]);
  free(vector);
  if (current)
    string_free(current);
  *argc = 0;
  return nullptr;
}

/* Modify the string substituting all the occurrences of the set of
 * characters specified in the 'from' string to the corresponding character
 * in the 'to' array.
 *
 * For instance: string_mapchars(mystring, "ho", "01", 2)
 * will have the effect of turning the string "hello" into "0ell1".
 *
 * The function returns the string_t string pointer, that is always the same
 * as the input pointer since no resize is needed. */
string_t string_mapchars(string_t s, const char *from, const char *to, size_t setlen) {
  size_t j, i, l = string_len(s);

  for (j = 0; j < l; j++) {
    for (i = 0; i < setlen; i++) {
      if (s[j] == from[i]) {
        s[j] = to[i];
        break;
      }
    }
  }
  return s;
}

/* Join an array of C strings using the specified separator (also a C string).
 * Returns the result as an string_t string. */
string_t string_join(char **argv, int argc, char *sep) {
  string_t join = string_empty();
  int j;

  for (j = 0; j < argc; j++) {
    join = string_cat(join, argv[j]);
    if (j != argc - 1)
      join = string_cat(join, sep);
  }
  return join;
}

/* Like string_join, but joins an array of SDS strings. */
string_t string_joinsds(string_t *argv, int argc, const char *sep, size_t seplen) {
  string_t join = string_empty();
  int j;

  for (j = 0; j < argc; j++) {
    join = string_catsds(join, argv[j]);
    if (j != argc - 1)
      join = string_catlen(join, sep, seplen);
  }
  return join;
}
