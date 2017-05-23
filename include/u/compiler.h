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

/*!\file compiler.h
 * \author Mattias Jansson <www.github.com/rampantpixels>
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_COMPILER_H__
# define U_COMPILER_H__

#include "platform.h"

/*!\defgroup Compiler
 * \brief Compiler features support
 * @{
 */

/*!\def COMPILER_CLANG
 * \bried Defined to 1 if compiling with clang, 0 otherwise
 */
#define COMPILER_CLANG 0

/*!\def COMPILER_MSVC
 * \bried Defined to 1 if compiling with Microsoft compiler, 0 otherwise
 */
#define COMPILER_GCC 0

/*!\def COMPILER_GCC
 * \bried Defined to 1 if compiling with GCC, 0 otherwise
 */
#define COMPILER_MSVC 0

/*!\def COMPILER_INTEL
 * \bried Defined to 1 if compiling with Intel compiler, 0 otherwise
 */
#define COMPILER_INTEL 0

/*!\def COMPILER_NAME
 * \brief A string naming the compiler used
 * 
 * \def COMPILER_DESCRIPTION
 * \brief A string with a more detailed description of the compiler used, name and version.
 * 
 * \def RESTRICT
 * \brief Restrict attribute, defined to nothing if compiler does not support restrict
 * 
 * \def thread_local
 * \brief Thread local attribute, defined to nothing if the compiler/platform/architecture does not
 * support thread local variables.
 * instead.
 * 
 * \def DEPRECATED
 * \brief Deprecated attribute, marking a function/variable as deprecated
 * 
 * \def FORCEINLINE
 * \brief Attribute to force function to be inlined
 * 
 * \def NOINLINE
 * \brief Attribute to prevent function from being inlined
 * 
 * \def PURECALL
 * \brief Attribute declaring function to be pure, meaning it has no effects except the return value
 * and the return value depends only on the parameters and/or global variables.
 * 
 * \def CONSTCALL
 * \brief Attribute declaring function to be const, meaning it does not examine any values except
 * the arguments, and has no effects except the return value. Basically this is just slightly
 * more strict class than the PURECALL attribute, since function is not allowed to read global
 * memory. Note that a function that has pointer arguments and examines the data pointed to
 * must not be declared const. Likewise, a function that calls a non-const function must
 * usually not be const. It does not make sense for a const function to return void.
 * 
 * \def ALIGN
 * \brief Variable or type attribute declaring the variable/type to have the specifiedmemory alignment
 * \param alignment Alignment
 * 
 * \def alignof
 * \brief Get the alignment of the given type or variable
 * \param type Type or variable
 * 
 * \def ALIGNED_STRUCT
 * \brief Declare an aligned struct type
 * \param name Struct name
 * \param alignment Alignment
 *
 * @}
 */

#define NX_STRINGIFY(x) #x
#define STRINGIFY(x) NX_STRINGIFY(x)
#define VERSION_NO(A, B, C) (((A)*10000UL)+(B)*100UL+(C))

#if defined(__clang__)
# undef  COMPILER_CLANG
# define COMPILER_CLANG 1
# define COMPILER_NAME "clang"
# define COMPILER_DESCRIPTION COMPILER_NAME " " #__clang_major__ "." #__clang_minor__
# define COMPILER_VERSION VERSION_NO(__clang_major__, __clang_minor__, __clang_patchlevel__)
# define restrict __restrict
# define thread_local _Thread_local
# define ATTR(x) __attribute__((__##x##__))
# define ATTR2(x,y) __attribute__((__##x##__(y)))
# define ATTR3(x,y,z) __attribute__((__##x##__(y,z)))
# define ATTR4(x,y,z,w) __attribute__((__##x##__(y,z,w)))
# define DEPRECATED ATTR(deprecated)
# define FORCEINLINE inline ATTR(always_inline)
# define NOINLINE ATTR(noinline)
# define PURECALL ATTR(pure)
# define CONSTCALL ATTR(const)
# define PRINTFCALL(start, num) ATTR4(format, printf, start, num)
# define ALIGN(alignment) ATTR2(aligned, alignment)
# define alignof(type) __alignof__(type)
# define ALIGNED_STRUCT(name, alignment) struct __attribute__((__aligned__(alignment))) name
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# if PLATFORM_WINDOWS
#   if (CLANG_VERSION < 30800)
#     error CLang 3.8 or later is required
#   endif
#   define STDCALL ATTR(stdcall)
#   ifndef __USE_MINGW_ANSI_STDIO
#     define __USE_MINGW_ANSI_STDIO 1
#   endif
#   ifndef _CRT_SECURE_NO_WARNINGS
#     define _CRT_SECURE_NO_WARNINGS 1
#   endif
#   ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#     define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#   endif
#   ifndef __MSVCRT_VERSION__
#     define __MSVCRT_VERSION__ 0x0800
#   endif
#   define USE_NO_MINGW_SETJMP_TWO_ARGS 1
#   if __has_warning("-Wunknown-pragmas")
#     pragma clang diagnostic ignored "-Wunknown-pragmas"
#   endif
#   if __has_warning("-Wformat-non-iso")
#     pragma clang diagnostic ignored "-Wformat-non-iso"
#   endif
# endif
# if __has_warning("-Wreserved-id-macro")
#   pragma clang diagnostic ignored "-Wreserved-id-macro"
# endif
# if __has_warning("-Wcovered-switch-default")
#   pragma clang diagnostic ignored "-Wcovered-switch-default"
# endif
#elif defined(__ICL) || defined(__ICC) || defined(__INTEL_COMPILER)
# undef  COMPILER_INTEL
# define COMPILER_INTEL 1
# define COMPILER_NAME "intel"
# if defined(__ICL)
#   define COMPILER_DESCRIPTION COMPILER_NAME " " #__ICL
# elif defined(__ICC)
#   define COMPILER_DESCRIPTION COMPILER_NAME " " #__ICC
# endif
# define COMPILER_VERSION VERSION_NO(__INTEL_COMPILER, 0, 0)
# define restrict __restrict
# define thread_local __declspec(thread)
# define ATTR(x)
# define ATTR2(x,y)
# define ATTR3(x,y,z)
# define ATTR4(x,y,z,w)
# define DEPRECATED
# define FORCEINLINE __forceinline
# define NOINLINE __declspec(noinline)
# define PURECALL
# define CONSTCALL
# define PRINTFCALL(start, num)
# define ALIGN(alignment) __declspec(align(alignment))
# define alignof(type) __alignof(type)
# define ALIGNED_STRUCT(name, alignment) ALIGN(alignment) struct name
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# if PLATFORM_WINDOWS
#   define STDCALL __stdcall
#   define va_copy(d,s) ((d)=(s))
# endif
# include <intrin.h>
#elif defined(_MSC_VER)
# undef  COMPILER_MSVC
# define COMPILER_MSVC 1
# define COMPILER_NAME "msvc"
# define COMPILER_DESCRIPTION COMPILER_NAME " " STRINGIFY(_MSC_VER)
# define COMPILER_VERSION _MSC_VER
# define ATTR(x)
# define ATTR2(x,y)
# define ATTR3(x,y,z)
# define ATTR4(x,y,z,w)
# define restrict __restrict
# define thread_local __declspec(thread)
# define DEPRECATED __declspec(deprecated)
# define FORCEINLINE __forceinline
# define NOINLINE __declspec(noinline)
# define NORETURN __declspec(noreturn)
# define PURECALL
# define CONSTCALL
# define PRINTFCALL(start, num)
# define ALIGN(alignment) __declspec(align(alignment))
# define alignof(type) __alignof(type)
# define ALIGNED_STRUCT(name, alignment) ALIGN(alignment) struct name
# define LIKELY(x) (x)
# define UNLIKELY(x) (x)
# pragma warning(disable : 4018)
# pragma warning(disable : 4127)
# pragma warning(disable : 4132)
# pragma warning(disable : 4146)
# pragma warning(disable : 4200)
# pragma warning(disable : 4204)
# pragma warning(disable : 4706)
# ifdef __cplusplus
#   pragma warning(disable : 4100)
#   pragma warning(disable : 4510)
#   pragma warning(disable : 4512)
#   pragma warning(disable : 4610)
# endif
# if PLATFORM_WINDOWS
#   define STDCALL __stdcall
# endif
# if defined(COMPILE) && COMPILE && !defined(_CRT_SECURE_NO_WARNINGS)
#   define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _LINT
#   define static_assert static_assert
# endif
# if _MSC_VER < 1800
#   define va_copy(d,s) ((d)=(s))
# endif
# include <intrin.h>
#elif defined(__GNUC__)
# undef  COMPILER_GCC
# define COMPILER_GCC 1
# define COMPILER_NAME "gcc"
# define COMPILER_DESCRIPTION COMPILER_NAME " " STRINGIFY(__GNUC__) "." STRINGIFY(__GNUC_MINOR__)
# define COMPILER_VERSION VERSION_NO(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
# define restrict __restrict
# define thread_local __thread
# define ATTR(x) __attribute__((__##x##__))
# define ATTR2(x,y) __attribute__((__##x##__(y)))
# define ATTR3(x,y,z) __attribute__((__##x##__(y,z)))
# define ATTR4(x,y,z,w) __attribute__((__##x##__(y,z,w)))
# define DEPRECATED ATTR(deprecated)
# define FORCEINLINE inline ATTR(always_inline)
# define NOINLINE ATTR(noinline)
# define PURECALL ATTR(pure)
# define CONSTCALL ATTR(const)
# define PRINTFCALL(start, num) ATTR4(format, printf, start, num)
# define ALIGN(alignment) ATTR2(aligned, alignment)
# define alignof(type) __alignof__(type)
# define ALIGNED_STRUCT(name, alignment) struct ALIGN(alignment) name
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# if COMPILER_VERSION >= 40600UL
#   define static_assert _Static_assert
# endif
# if PLATFORM_WINDOWS
#   define STDCALL ATTR(stdcall)
#   ifndef __USE_MINGW_ANSI_STDIO
#     define __USE_MINGW_ANSI_STDIO 1
#   endif
#   ifndef _CRT_SECURE_NO_WARNINGS
#     define _CRT_SECURE_NO_WARNINGS 1
#   endif
#   ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#     define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#   endif
#   ifndef __MSVCRT_VERSION__
#     define __MSVCRT_VERSION__ 0x0800
#   endif
#   pragma GCC diagnostic ignored "-Wformat"
#   pragma GCC diagnostic ignored "-Wformat-extra-args"
#   pragma GCC diagnostic ignored "-Wpedantic"
# endif
#else
# warning Unknown compiler
# define COMPILER_NAME "unknown"
# define COMPILER_DESCRIPTION "unknown"
# define restrict
# define thread_local
# define DEPRECATED
# define FORCEINLINE
# define NOINLINE
# define PURECALL
# define CONSTCALL
# define ALIGN
# define alignof
# define ALIGNED_STRUCT(name, alignment) struct name
# define LIKELY(x) (x)
# define UNLIKELY(x) (x)
#endif

#ifndef static_assert
# define static_assert(EXPR, DIAGSTR) \
  extern char const u_compiletime_assert[ \
   sizeof((void const*[3*(!!(EXPR)) - 1]){ \
     &u_compiletime_assert, \
     "static assertion failed: " STRINGIFY(EXPR) ", " DIAGSTR}) \
  ]
  extern char const u_compiletime_assert[sizeof(void const*[2])];
#endif

#if PLATFORM_POSIX
# ifndef _GNU_SOURCE
#   define _GNU_SOURCE
# endif
#endif

#endif /* U_COMPILER_H__ */
