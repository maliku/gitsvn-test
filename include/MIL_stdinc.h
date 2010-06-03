/*
    MIL - A embedded system graphics Middle Interface Layer.
    Copyright (C) 2009-2010 Dong Kai

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Dong Kai
    libmilui@gmail.com
*/

/** @file MIL_stdinc.h
 *  This is a general header that includes C language support
 */

#ifndef _MIL_stdinc_h
#define _MIL_stdinc_h

#include "MIL_config.h"


#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#if defined(STDC_HEADERS)
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
#else
# if defined(HAVE_STDLIB_H)
#  include <stdlib.h>
# elif defined(HAVE_MALLOC_H)
#  include <malloc.h>
# endif
# if defined(HAVE_STDDEF_H)
#  include <stddef.h>
# endif
# if defined(HAVE_STDARG_H)
#  include <stdarg.h>
# endif
#endif
#ifdef HAVE_STRING_H
# if !defined(STDC_HEADERS) && defined(HAVE_MEMORY_H)
#  include <memory.h>
# endif
# include <string.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif
#if defined(HAVE_INTTYPES_H)
# include <inttypes.h>
#elif defined(HAVE_STDINT_H)
# include <stdint.h>
#endif
#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif
#if defined(HAVE_ICONV) && defined(HAVE_ICONV_H)
# include <iconv.h>
#endif

/** The number of elements in an array */
#define MIL_arraysize(array)	(sizeof(array)/sizeof(array[0]))
#define MIL_TABLESIZE(table)	MIL_arraysize(table)

/* Use proper C++ casts when compiled as C++ to be compatible with the option
 -Wold-style-cast of GCC (and -Werror=old-style-cast in GCC 4.2 and above. */
#ifdef __cplusplus
#define MIL_reinterpret_cast(type, expression) reinterpret_cast<type>(expression)
#define MIL_static_cast(type, expression) static_cast<type>(expression)
#else
#define MIL_reinterpret_cast(type, expression) ((type)(expression))
#define MIL_static_cast(type, expression) ((type)(expression))
#endif

/** @name Basic data types */
/*@{*/
typedef enum {
	MIL_FALSE = 0,
	MIL_TRUE  = 1
} MIL_Bool;

typedef int8_t		Sint8;
typedef uint8_t		Uint8;
typedef int16_t		Sint16;
typedef uint16_t	Uint16;
typedef int32_t		Sint32;
typedef uint32_t	Uint32;

#ifdef MIL_HAS_64BIT_TYPE
typedef int64_t		Sint64;
#ifndef SYMBIAN32_GCCE
typedef uint64_t	Uint64;
#endif
#else
/* This is really just a hack to prevent the compiler from complaining */
typedef struct {
	Uint32 hi;
	Uint32 lo;
} Uint64, Sint64;
#endif

/*@}*/
typedef enum  {
    MIL_GENERIC_ERROR               = -1,
    MIL_OK                          = 0,
    MIL_INVALID_PARAMETER           = 1,
    MIL_OUT_OF_MEMORY               = 2,
    MIL_IMPL_BY_SUBCLASS            = 3,
    MIL_OBJECT_BUSY                 = 4,
    MIL_INSUFFICIENT_BUFFER         = 5,
    MIL_NOT_IMPLEMENTED             = 6,
    MIL_SYSTEM_CALL_ERROR           = 7,
    MIL_WRONG_STATE                 = 8,
    MIL_ABORTED                     = 9,
    MIL_FILE_NOT_FOUND               = 10,
    MIL_VALUE_OVERFLOW               = 11,
    MIL_ACCESS_DENIED                = 12,
    MIL_UNKNOWN_FILE_FORMAT          = 13,
    MIL_FONT_FAMILY_NOT_FOUND          = 14,
    MIL_FONT_STYLE_NOT_FOUND           = 15,
    MIL_NOT_TRUE_TYPE_FONT             = 16,
    MIL_UNSUPPORTED_VERSION   = 17,
    MIL_NOT_INITIALIZED       = 18,
    MIL_PropertyNotFound            = 19,
    MIL_PropertyNotSupported        = 20,
    MIL_ProfileNotFound             = 21 
} MIL_Status;

/** @name Make sure the types really have the right sizes */
/*@{*/
#define MIL_COMPILE_TIME_ASSERT(name, x)               \
       typedef int MIL_dummy_ ## name[(x) * 2 - 1]

MIL_COMPILE_TIME_ASSERT(uint8, sizeof(Uint8) == 1);
MIL_COMPILE_TIME_ASSERT(sint8, sizeof(Sint8) == 1);
MIL_COMPILE_TIME_ASSERT(uint16, sizeof(Uint16) == 2);
MIL_COMPILE_TIME_ASSERT(sint16, sizeof(Sint16) == 2);
MIL_COMPILE_TIME_ASSERT(uint32, sizeof(Uint32) == 4);
MIL_COMPILE_TIME_ASSERT(sint32, sizeof(Sint32) == 4);
MIL_COMPILE_TIME_ASSERT(uint64, sizeof(Uint64) == 8);
MIL_COMPILE_TIME_ASSERT(sint64, sizeof(Sint64) == 8);
/*@}*/

/** @name Enum Size Check
 *  Check to make sure enums are the size of ints, for structure packing.
 *  For both Watcom C/C++ and Borland C/C++ the compiler option that makes
 *  enums having the size of an int must be enabled.
 *  This is "-b" for Borland C/C++ and "-ei" for Watcom C/C++ (v11).
 */
/* Enable enums always int in CodeWarrior (for MPW use "-enum int") */
#ifdef __MWERKS__
#pragma enumsalwaysint on
#endif

typedef enum {
	DUMMY_ENUM_VALUE
} MIL_DUMMY_ENUM;

#ifndef __NDS__
MIL_COMPILE_TIME_ASSERT(enum, sizeof(MIL_DUMMY_ENUM) == sizeof(int));
#endif
/*@}*/

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_MALLOC
#define MIL_malloc	malloc
#else
extern DECLSPEC void * MILCALL MIL_malloc(size_t size);
#endif

#ifdef HAVE_CALLOC
#define MIL_calloc	calloc
#else
extern DECLSPEC void * MILCALL MIL_calloc(size_t nmemb, size_t size);
#endif

#ifdef HAVE_REALLOC
#define MIL_realloc	realloc
#else
extern DECLSPEC void * MILCALL MIL_realloc(void *mem, size_t size);
#endif

#ifdef HAVE_FREE
#define MIL_free	free
#else
extern DECLSPEC void MILCALL MIL_free(void *mem);
#endif

#if defined(HAVE_ALLOCA) && !defined(alloca)
# if defined(HAVE_ALLOCA_H)
#  include <alloca.h>
# elif defined(__GNUC__)
#  define alloca __builtin_alloca
# elif defined(_MSC_VER)
#  include <malloc.h>
#  define alloca _alloca
# elif defined(__WATCOMC__)
#  include <malloc.h>
# elif defined(__BORLANDC__)
#  include <malloc.h>
# elif defined(__DMC__)
#  include <stdlib.h>
# elif defined(__AIX__)
  #pragma alloca
# elif defined(__MRC__)
   void *alloca (unsigned);
# else
   char *alloca ();
# endif
#endif
#ifdef HAVE_ALLOCA
#define MIL_stack_alloc(type, count)    (type*)alloca(sizeof(type)*(count))
#define MIL_stack_free(data)
#else
#define MIL_stack_alloc(type, count)    (type*)MIL_malloc(sizeof(type)*(count))
#define MIL_stack_free(data)            MIL_free(data)
#endif

#ifdef HAVE_GETENV
#define MIL_getenv	getenv
#else
extern DECLSPEC char * MILCALL MIL_getenv(const char *name);
#endif

#ifdef HAVE_PUTENV
#define MIL_putenv	putenv
#else
extern DECLSPEC int MILCALL MIL_putenv(const char *variable);
#endif

#ifdef HAVE_QSORT
#define MIL_qsort	qsort
#else
extern DECLSPEC void MILCALL MIL_qsort(void *base, size_t nmemb, size_t size,
           int (*compare)(const void *, const void *));
#endif

#ifdef HAVE_ABS
#define MIL_abs		abs
#else
#define MIL_abs(X)	((X) < 0 ? -(X) : (X))
#endif

#define MIL_min(x, y)	(((x) < (y)) ? (x) : (y))
#define MIL_max(x, y)	(((x) > (y)) ? (x) : (y))

#ifdef HAVE_CTYPE_H
#define MIL_isdigit(X)  isdigit(X)
#define MIL_isspace(X)  isspace(X)
#define MIL_toupper(X)  toupper(X)
#define MIL_tolower(X)  tolower(X)
#else
#define MIL_isdigit(X)  (((X) >= '0') && ((X) <= '9'))
#define MIL_isspace(X)  (((X) == ' ') || ((X) == '\t') || ((X) == '\r') || ((X) == '\n'))
#define MIL_toupper(X)  (((X) >= 'a') && ((X) <= 'z') ? ('A'+((X)-'a')) : (X))
#define MIL_tolower(X)  (((X) >= 'A') && ((X) <= 'Z') ? ('a'+((X)-'A')) : (X))
#endif

#if defined(__GNUC__) && defined(i386)
#define ASM_memset2(s, c, count)  \
do {                            \
    int eax, edi, ecx;          \
    __asm__ __volatile__ (      \
            "cld\n\t"           \
            "rep\n\t"           \
            "stosw"             \
            : "=&D" (edi), "=&a" (eax), "=&c" (ecx) \
            : "0"(s), "1"(c), "2"(count)            \
            : "memory");        \
} while (0)

#define ASM_memset4(s, c, count)    \
do {                                \
    int eax, edi, ecx;              \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "rep\n\t"               \
            "stosl"                 \
            : "=&D" (edi), "=&a" (eax), "=&c" (ecx) \
            : "0"(s), "1"(c), "2"(count)    \
            : "memory");            \
} while (0)

#define ASM_memxorset(s, c, n)      \
do {                                \
    int ecx, edx, esi, edi;         \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsb\n\t"             \
            "xorb %%dl, %%al\n\t"   \
            "stosb\n\t"             \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx), "=&d" (edx)    \
            : "0" (s), "1" (s), "2" (n), "3" (c)    \
            : "al");            \
} while (0)

#define ASM_memxorset2(s, c, n)     \
do {                                \
    int ecx, edx, esi, edi;         \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsw\n\t"             \
            "xorw %%dx, %%ax\n\t"   \
            "stosw\n\t"             \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx), "=&d" (edx)    \
            : "0" (s), "1" (s), "2" (n), "3" (c)    \
            : "ax");            \
} while (0)

#define ASM_memxorset4(s, c, n)     \
do {                                \
    int ecx, edx, esi, edi;         \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsl\n\t"             \
            "xorl %%edx, %%eax\n\t" \
            "stosl\n\t"             \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx), "=&d" (edx)    \
            : "0" (s), "1" (s), "2" (n), "3" (c)    \
            : "eax");            \
} while (0)

#define ASM_memandset4(s, c, n)     \
do {                                \
    int ecx, edx, esi, edi;         \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsl\n\t"             \
            "andl %%edx, %%eax\n\t" \
            "stosl\n\t"             \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx), "=&d" (edx)    \
            : "0" (s), "1" (s), "2" (n), "3" (c)    \
            : "eax");            \
} while (0)

#define ASM_memorset4(s, c, n)      \
do {                                \
    int ecx, edx, esi, edi;         \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsl\n\t"             \
            "orl %%edx, %%eax\n\t"  \
            "stosl\n\t"             \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx), "=&d" (edx)    \
            : "0" (s), "1" (s), "2" (n), "3" (c)    \
            : "eax");            \
} while (0)

#define ASM_memandcpy4(dst, src, n) \
do {                                \
    int edi, esi, ecx;              \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsl\n\t"             \
            "andl %%eax, (%%edi)\n\t"   \
            "addl $4, %%edi\n\t"    \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx) \
            : "0" (dst), "1" (src), "2" (n) \
            : "eax");            \
} while (0)

#define ASM_memorcpy4(dst, src, n)  \
do {                                \
    int edi, esi, ecx;              \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsl\n\t"             \
            "orl %%eax, (%%edi)\n\t"    \
            "addl $4, %%edi\n\t"    \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx) \
            : "0" (dst), "1" (src), "2" (n) \
            : "eax");            \
} while (0)

#define ASM_memxorcpy4(dst, src, n) \
do {                                \
    int edi, esi, ecx;              \
    __asm__ __volatile__ (          \
            "cld\n\t"               \
            "1:\n\t"                \
            "lodsl\n\t"             \
            "xorl %%eax, (%%edi)\n\t"   \
            "addl $4, %%edi\n\t"    \
            "loop 1b\n\t"           \
            : "=&D" (edi), "=&S" (esi), "=&c" (ecx) \
            : "0" (dst), "1" (src), "2" (n) \
            : "eax");            \
} while (0)
#endif /* defined(__GNUC__) && defined(i386) */


#ifdef HAVE_MEMSET
#define MIL_memset      memset
#else
extern DECLSPEC void * MILCALL MIL_memset(void *dst, int c, size_t len);
#endif

#if defined(__GNUC__) && defined(i386)
#define MIL_memset4(dst, val, len)				\
do {								\
	int u0, u1, u2;						\
	__asm__ __volatile__ (					\
		"cld\n\t"					\
		"rep ; stosl\n\t"				\
		: "=&D" (u0), "=&a" (u1), "=&c" (u2)		\
		: "0" (dst), "1" (val), "2" (MIL_static_cast(Uint32, len))	\
		: "memory" );					\
} while(0)
#endif
#ifndef MIL_memset4
#define MIL_memset4(dst, val, len)		\
do {						\
	unsigned _count = (len);		\
	unsigned _n = (_count + 3) / 4;		\
	Uint32 *_p = MIL_static_cast(Uint32 *, dst);	\
	Uint32 _val = (val);			\
	if (len == 0) break;			\
        switch (_count % 4) {			\
        case 0: do {    *_p++ = _val;		\
        case 3:         *_p++ = _val;		\
        case 2:         *_p++ = _val;		\
        case 1:         *_p++ = _val;		\
		} while ( --_n );		\
	}					\
} while(0)
#endif

/* We can count on memcpy existing on Mac OS X and being well-tuned. */
#if defined(__MACH__) && defined(__APPLE__)
#define MIL_memcpy(dst, src, len) memcpy(dst, src, len)
#elif defined(__GNUC__) && defined(i386)
#define MIL_memcpy(dst, src, len)					  \
do {									  \
	int u0, u1, u2;						  	  \
	__asm__ __volatile__ (						  \
		"cld\n\t"						  \
		"rep ; movsl\n\t"					  \
		"testb $2,%b4\n\t"					  \
		"je 1f\n\t"						  \
		"movsw\n"						  \
		"1:\ttestb $1,%b4\n\t"					  \
		"je 2f\n\t"						  \
		"movsb\n"						  \
		"2:"							  \
		: "=&c" (u0), "=&D" (u1), "=&S" (u2)			  \
		: "0" (MIL_static_cast(unsigned, len)/4), "q" (len), "1" (dst),"2" (src) \
		: "memory" );						  \
} while(0)
#endif
#ifndef MIL_memcpy
#ifdef HAVE_MEMCPY
#define MIL_memcpy      memcpy
#elif defined(HAVE_BCOPY)
#define MIL_memcpy(d, s, n)	bcopy((s), (d), (n))
#else
extern DECLSPEC void * MILCALL MIL_memcpy(void *dst, const void *src, size_t len);
#endif
#endif

/* We can count on memcpy existing on Mac OS X and being well-tuned. */
#if defined(__MACH__) && defined(__APPLE__)
#define MIL_memcpy4(dst, src, len) memcpy(dst, src, (len)*4)
#elif defined(__GNUC__) && defined(i386)
#define MIL_memcpy4(dst, src, len)				\
do {								\
	int ecx, edi, esi;					\
	__asm__ __volatile__ (					\
		"cld\n\t"					\
		"rep ; movsl"					\
		: "=&c" (ecx), "=&D" (edi), "=&S" (esi)		\
		: "0" (MIL_static_cast(unsigned, len)), "1" (dst), "2" (src)	\
		: "memory" );					\
} while(0)
#endif
#ifndef MIL_memcpy4
#define MIL_memcpy4(dst, src, len)	MIL_memcpy(dst, src, (len) << 2)
#endif

#if defined(__GNUC__) && defined(i386)
#define MIL_revcpy(dst, src, len)			\
do {							\
	int u0, u1, u2;					\
	char *dstp = MIL_static_cast(char *, dst);	\
	char *srcp = MIL_static_cast(char *, src);	\
	int n = (len);					\
	if ( n >= 4 ) {					\
	__asm__ __volatile__ (				\
		"std\n\t"				\
		"rep ; movsl\n\t"			\
		"cld\n\t"				\
		: "=&c" (u0), "=&D" (u1), "=&S" (u2)	\
		: "0" (n >> 2),				\
		  "1" (dstp+(n-4)), "2" (srcp+(n-4))	\
		: "memory" );				\
	}						\
	switch (n & 3) {				\
		case 3: dstp[2] = srcp[2];		\
		case 2: dstp[1] = srcp[1];		\
		case 1: dstp[0] = srcp[0];		\
			break;				\
		default:				\
			break;				\
	}						\
} while(0)
#endif
#ifndef MIL_revcpy
extern DECLSPEC void * MILCALL MIL_revcpy(void *dst, const void *src, size_t len);
#endif

#ifdef HAVE_MEMMOVE
#define MIL_memmove     memmove
#elif defined(HAVE_BCOPY)
#define MIL_memmove(d, s, n)	bcopy((s), (d), (n))
#else
#define MIL_memmove(dst, src, len)			\
do {							\
	if ( dst < src ) {				\
		MIL_memcpy(dst, src, len);		\
	} else {					\
		MIL_revcpy(dst, src, len);		\
	}						\
} while(0)
#endif

#ifdef HAVE_MEMCMP
#define MIL_memcmp      memcmp
#else
extern DECLSPEC int MILCALL MIL_memcmp(const void *s1, const void *s2, size_t len);
#endif

#ifdef HAVE_STRLEN
#define MIL_strlen      strlen
#else
extern DECLSPEC size_t MILCALL MIL_strlen(const char *string);
#endif

#ifdef HAVE_STRLCPY
#define MIL_strlcpy     strlcpy
#else
extern DECLSPEC size_t MILCALL MIL_strlcpy(char *dst, const char *src, size_t maxlen);
#endif

#ifdef HAVE_STRLCAT
#define MIL_strlcat    strlcat
#else
extern DECLSPEC size_t MILCALL MIL_strlcat(char *dst, const char *src, size_t maxlen);
#endif

#ifdef HAVE_STRDUP
#define MIL_strdup     strdup
#else
extern DECLSPEC char * MILCALL MIL_strdup(const char *string);
#endif

#ifdef HAVE__STRREV
#define MIL_strrev      _strrev
#else
extern DECLSPEC char * MILCALL MIL_strrev(char *string);
#endif

#ifdef HAVE__STRUPR
#define MIL_strupr      _strupr
#else
extern DECLSPEC char * MILCALL MIL_strupr(char *string);
#endif

#ifdef HAVE__STRLWR
#define MIL_strlwr      _strlwr
#else
extern DECLSPEC char * MILCALL MIL_strlwr(char *string);
#endif

#ifdef HAVE_STRCHR
#define MIL_strchr      strchr
#elif defined(HAVE_INDEX)
#define MIL_strchr      index
#else
extern DECLSPEC char * MILCALL MIL_strchr(const char *string, int c);
#endif

#ifdef HAVE_STRRCHR
#define MIL_strrchr     strrchr
#elif defined(HAVE_RINDEX)
#define MIL_strrchr     rindex
#else
extern DECLSPEC char * MILCALL MIL_strrchr(const char *string, int c);
#endif

#ifdef HAVE_STRSTR
#define MIL_strstr      strstr
#else
extern DECLSPEC char * MILCALL MIL_strstr(const char *haystack, const char *needle);
#endif

#ifdef HAVE_ITOA
#define MIL_itoa        itoa
#else
#define MIL_itoa(value, string, radix)	MIL_ltoa((long)value, string, radix)
#endif

#ifdef HAVE__LTOA
#define MIL_ltoa        _ltoa
#else
extern DECLSPEC char * MILCALL MIL_ltoa(long value, char *string, int radix);
#endif

#ifdef HAVE__UITOA
#define MIL_uitoa       _uitoa
#else
#define MIL_uitoa(value, string, radix)	MIL_ultoa((long)value, string, radix)
#endif

#ifdef HAVE__ULTOA
#define MIL_ultoa       _ultoa
#else
extern DECLSPEC char * MILCALL MIL_ultoa(unsigned long value, char *string, int radix);
#endif

#ifdef HAVE_STRTOL
#define MIL_strtol      strtol
#else
extern DECLSPEC long MILCALL MIL_strtol(const char *string, char **endp, int base);
#endif

#ifdef HAVE_STRTOUL
#define MIL_strtoul      strtoul
#else
extern DECLSPEC unsigned long MILCALL MIL_strtoul(const char *string, char **endp, int base);
#endif

#ifdef MIL_HAS_64BIT_TYPE

#ifdef HAVE__I64TOA
#define MIL_lltoa       _i64toa
#else
extern DECLSPEC char* MILCALL MIL_lltoa(Sint64 value, char *string, int radix);
#endif

#ifdef HAVE__UI64TOA
#define MIL_ulltoa      _ui64toa
#else
extern DECLSPEC char* MILCALL MIL_ulltoa(Uint64 value, char *string, int radix);
#endif

#ifdef HAVE_STRTOLL
#define MIL_strtoll     strtoll
#else
extern DECLSPEC Sint64 MILCALL MIL_strtoll(const char *string, char **endp, int base);
#endif

#ifdef HAVE_STRTOULL
#define MIL_strtoull     strtoull
#else
extern DECLSPEC Uint64 MILCALL MIL_strtoull(const char *string, char **endp, int base);
#endif

#endif /* MIL_HAS_64BIT_TYPE */

#ifdef HAVE_STRTOD
#define MIL_strtod      strtod
#else
extern DECLSPEC double MILCALL MIL_strtod(const char *string, char **endp);
#endif

#ifdef HAVE_ATOI
#define MIL_atoi        atoi
#else
#define MIL_atoi(X)     MIL_strtol(X, NULL, 0)
#endif

#ifdef HAVE_ATOF
#define MIL_atof        atof
#else
#define MIL_atof(X)     MIL_strtod(X, NULL)
#endif

#ifdef HAVE_STRCMP
#define MIL_strcmp      strcmp
#else
extern DECLSPEC int MILCALL MIL_strcmp(const char *str1, const char *str2);
#endif

#ifdef HAVE_STRNCMP
#define MIL_strncmp     strncmp
#else
extern DECLSPEC int MILCALL MIL_strncmp(const char *str1, const char *str2, size_t maxlen);
#endif

#ifdef HAVE_STRCASECMP
#define MIL_strcasecmp  strcasecmp
#elif defined(HAVE__STRICMP)
#define MIL_strcasecmp  _stricmp
#else
extern DECLSPEC int MILCALL MIL_strcasecmp(const char *str1, const char *str2);
#endif

#ifdef HAVE_STRNCASECMP
#define MIL_strncasecmp strncasecmp
#elif defined(HAVE__STRNICMP)
#define MIL_strncasecmp _strnicmp
#else
extern DECLSPEC int MILCALL MIL_strncasecmp(const char *str1, const char *str2, size_t maxlen);
#endif

#ifdef HAVE_SSCANF
#define MIL_sscanf      sscanf
#else
extern DECLSPEC int MILCALL MIL_sscanf(const char *text, const char *fmt, ...);
#endif

#ifdef HAVE_SNPRINTF
#define MIL_snprintf    snprintf
#else
extern DECLSPEC int MILCALL MIL_snprintf(char *text, size_t maxlen, const char *fmt, ...);
#endif

#ifdef HAVE_VSNPRINTF
#define MIL_vsnprintf   vsnprintf
#else
extern DECLSPEC int MILCALL MIL_vsnprintf(char *text, size_t maxlen, const char *fmt, va_list ap);
#endif

/** @name MIL_ICONV Error Codes
 *  The MIL implementation of iconv() returns these error codes 
 */
/*@{*/
#define MIL_ICONV_ERROR		(size_t)-1
#define MIL_ICONV_E2BIG		(size_t)-2
#define MIL_ICONV_EILSEQ	(size_t)-3
#define MIL_ICONV_EINVAL	(size_t)-4
/*@}*/

#if defined(HAVE_ICONV) && defined(HAVE_ICONV_H)
#define MIL_iconv_t     iconv_t
#define MIL_iconv_open  iconv_open
#define MIL_iconv_close iconv_close
#else
typedef struct _MIL_iconv_t *MIL_iconv_t;
extern DECLSPEC MIL_iconv_t MILCALL MIL_iconv_open(const char *tocode, const char *fromcode);
extern DECLSPEC int MILCALL MIL_iconv_close(MIL_iconv_t cd);
#endif
extern DECLSPEC size_t MILCALL MIL_iconv(MIL_iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
/** This function converts a string between encodings in one pass, returning a
 *  string that must be freed with MIL_free() or NULL on error.
 */
extern DECLSPEC char * MILCALL MIL_iconv_string(const char *tocode, const char *fromcode, const char *inbuf, size_t inbytesleft);
#define MIL_iconv_utf8_locale(S)	MIL_iconv_string("", "UTF-8", S, MIL_strlen(S)+1)
#define MIL_iconv_utf8_ucs2(S)		(Uint16 *)MIL_iconv_string("UCS-2", "UTF-8", S, MIL_strlen(S)+1)
#define MIL_iconv_utf8_ucs4(S)		(Uint32 *)MIL_iconv_string("UCS-4", "UTF-8", S, MIL_strlen(S)+1)

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _MIL_stdinc_h */
