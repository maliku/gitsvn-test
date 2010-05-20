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
#include "MIL_config.h"

/* This file contains portable string manipulation functions for MIL */

#include "MIL_stdinc.h"


#define MIL_isupperhex(X)   (((X) >= 'A') && ((X) <= 'F'))
#define MIL_islowerhex(X)   (((X) >= 'a') && ((X) <= 'f'))

#if !defined(HAVE_SSCANF) || !defined(HAVE_STRTOL)
static size_t MIL_ScanLong(const char *text, int radix, long *valuep)
{
    const char *textstart = text;
    long value = 0;
    MIL_Bool negative = MIL_FALSE;

    if ( *text == '-' ) {
        negative = MIL_TRUE;
        ++text;
    }
    if ( radix == 16 && MIL_strncmp(text, "0x", 2) == 0 ) {
        text += 2;
    }
    for ( ; ; ) {
        int v;
        if ( MIL_isdigit((unsigned char) *text) ) {
            v = *text - '0';
        } else if ( radix == 16 && MIL_isupperhex(*text) ) {
            v = 10 + (*text - 'A');
        } else if ( radix == 16 && MIL_islowerhex(*text) ) {
            v = 10 + (*text - 'a');
        } else {
            break;
        }
        value *= radix;
        value += v;
        ++text;
    }
    if ( valuep ) {
        if ( negative && value ) {
            *valuep = -value;
        } else {
            *valuep = value;
        }
    }
    return (text - textstart);
}
#endif

#if !defined(HAVE_SSCANF) || !defined(HAVE_STRTOUL) || !defined(HAVE_STRTOD)
static size_t MIL_ScanUnsignedLong(const char *text, int radix, unsigned long *valuep)
{
    const char *textstart = text;
    unsigned long value = 0;

    if ( radix == 16 && MIL_strncmp(text, "0x", 2) == 0 ) {
        text += 2;
    }
    for ( ; ; ) {
        int v;
        if ( MIL_isdigit((unsigned char) *text) ) {
            v = *text - '0';
        } else if ( radix == 16 && MIL_isupperhex(*text) ) {
            v = 10 + (*text - 'A');
        } else if ( radix == 16 && MIL_islowerhex(*text) ) {
            v = 10 + (*text - 'a');
        } else {
            break;
        }
        value *= radix;
        value += v;
        ++text;
    }
    if ( valuep ) {
        *valuep = value;
    }
    return (text - textstart);
}
#endif

#ifndef HAVE_SSCANF
static size_t MIL_ScanUintPtrT(const char *text, int radix, uintptr_t *valuep)
{
    const char *textstart = text;
    uintptr_t value = 0;

    if ( radix == 16 && MIL_strncmp(text, "0x", 2) == 0 ) {
        text += 2;
    }
    for ( ; ; ) {
        int v;
        if ( MIL_isdigit((unsigned char) *text) ) {
            v = *text - '0';
        } else if ( radix == 16 && MIL_isupperhex(*text) ) {
            v = 10 + (*text - 'A');
        } else if ( radix == 16 && MIL_islowerhex(*text) ) {
            v = 10 + (*text - 'a');
        } else {
            break;
        }
        value *= radix;
        value += v;
        ++text;
    }
    if ( valuep ) {
        *valuep = value;
    }
    return (text - textstart);
}
#endif

#ifdef MIL_HAS_64BIT_TYPE
#if !defined(HAVE_SSCANF) || !defined(HAVE_STRTOLL)
static size_t MIL_ScanLongLong(const char *text, int radix, Sint64 *valuep)
{
    const char *textstart = text;
    Sint64 value = 0;
    MIL_Bool negative = MIL_FALSE;

    if ( *text == '-' ) {
        negative = MIL_TRUE;
        ++text;
    }
    if ( radix == 16 && MIL_strncmp(text, "0x", 2) == 0 ) {
        text += 2;
    }
    for ( ; ; ) {
        int v;
        if ( MIL_isdigit((unsigned char) *text) ) {
            v = *text - '0';
        } else if ( radix == 16 && MIL_isupperhex(*text) ) {
            v = 10 + (*text - 'A');
        } else if ( radix == 16 && MIL_islowerhex(*text) ) {
            v = 10 + (*text - 'a');
        } else {
            break;
        }
        value *= radix;
        value += v;
        ++text;
    }
    if ( valuep ) {
        if ( negative && value ) {
            *valuep = -value;
        } else {
            *valuep = value;
        }
    }
    return (text - textstart);
}
#endif

#if !defined(HAVE_SSCANF) || !defined(HAVE_STRTOULL)
static size_t MIL_ScanUnsignedLongLong(const char *text, int radix, Uint64 *valuep)
{
    const char *textstart = text;
    Uint64 value = 0;

    if ( radix == 16 && MIL_strncmp(text, "0x", 2) == 0 ) {
        text += 2;
    }
    for ( ; ; ) {
        int v;
        if ( MIL_isdigit((unsigned char) *text) ) {
            v = *text - '0';
        } else if ( radix == 16 && MIL_isupperhex(*text) ) {
            v = 10 + (*text - 'A');
        } else if ( radix == 16 && MIL_islowerhex(*text) ) {
            v = 10 + (*text - 'a');
        } else {
            break;
        }
        value *= radix;
        value += v;
        ++text;
    }
    if ( valuep ) {
        *valuep = value;
    }
    return (text - textstart);
}
#endif
#endif /* MIL_HAS_64BIT_TYPE */

#if !defined(HAVE_SSCANF) || !defined(HAVE_STRTOD)
static size_t MIL_ScanFloat(const char *text, double *valuep)
{
    const char *textstart = text;
    unsigned long lvalue = 0;
    double value = 0.0;
    MIL_Bool negative = MIL_FALSE;

    if ( *text == '-' ) {
        negative = MIL_TRUE;
        ++text;
    }
    text += MIL_ScanUnsignedLong(text, 10, &lvalue);
    value += lvalue;
    if ( *text == '.' ) {
        int mult = 10;
        ++text;
        while ( MIL_isdigit((unsigned char) *text) ) {
            lvalue = *text - '0';
            value += (double)lvalue / mult;
            mult *= 10;
            ++text;
        }
    }
    if ( valuep ) {
        if ( negative && value ) {
            *valuep = -value;
        } else {
            *valuep = value;
        }
    }
    return (text - textstart);
}
#endif

#ifndef MIL_memset
void *MIL_memset(void *dst, int c, size_t len)
{
    size_t left = (len % 4);
    if ( len >= 4 ) {
        Uint32 value = 0;
        Uint32 *dstp = (Uint32 *)dst;
        int i;
        for (i = 0; i < 4; ++i) {
            value <<= 8;
            value |= c;
        }
        len /= 4;
        while ( len-- ) {
            *dstp++ = value;
        }
    }
    if ( left > 0 ) {
        Uint8 value = (Uint8)c;
        Uint8 *dstp = (Uint8 *)dst;
	switch(left) {
	case 3:
            *dstp++ = value;
	case 2:
            *dstp++ = value;
	case 1:
            *dstp++ = value;
        }
    }
    return dst;
}
#endif

#ifndef MIL_memcpy
void *MIL_memcpy(void *dst, const void *src, size_t len)
{
    char *srcp = (char *)src;
    char *dstp = (char *)dst;
    while ( len-- ) {
        *dstp++ = *srcp++;
    }
    return dst;
}
#endif

#ifndef MIL_revcpy
void *MIL_revcpy(void *dst, const void *src, size_t len)
{
    char *srcp = (char *)src;
    char *dstp = (char *)dst;
    srcp += len-1;
    dstp += len-1;
    while ( len-- ) {
        *dstp-- = *srcp--;
    }
    return dst;
}
#endif

#ifndef MIL_memcmp
int MIL_memcmp(const void *s1, const void *s2, size_t len)
{
    char *s1p = (char *)s1;
    char *s2p = (char *)s2;
    while ( len-- ) {
        if ( *s1p != *s2p ) {
            return (*s1p - *s2p);
    }
    ++s1p;
    ++s2p;
    }
    return 0;
}
#endif

#ifndef HAVE_STRLEN
size_t MIL_strlen(const char *string)
{
    size_t len = 0;
    while ( *string++ ) {
        ++len;
    }
    return len;
}
#endif

#ifndef HAVE_STRLCPY
size_t MIL_strlcpy(char *dst, const char *src, size_t maxlen)
{
    size_t srclen = MIL_strlen(src);
    if ( maxlen > 0 ) {
        size_t len = MIL_min(srclen, maxlen-1);
        MIL_memcpy(dst, src, len);
        dst[len] = '\0';
    }
    return srclen;
}
#endif

#ifndef HAVE_STRLCAT
size_t MIL_strlcat(char *dst, const char *src, size_t maxlen)
{
    size_t dstlen = MIL_strlen(dst);
    size_t srclen = MIL_strlen(src);
    if ( dstlen < maxlen ) {
        MIL_strlcpy(dst+dstlen, src, maxlen-dstlen);
    }
    return dstlen+srclen;
}
#endif

#ifndef HAVE_STRDUP
char *MIL_strdup(const char *string)
{
    size_t len = MIL_strlen(string)+1;
    char *newstr = MIL_malloc(len);
    if ( newstr ) {
        MIL_strlcpy(newstr, string, len);
    }
    return newstr;
}
#endif

#ifndef HAVE__STRREV
char *MIL_strrev(char *string)
{
    size_t len = MIL_strlen(string);
    char *a = &string[0];
    char *b = &string[len-1];
    len /= 2;
    while ( len-- ) {
        char c = *a;
        *a++ = *b;
        *b-- = c;
    }
    return string;
}
#endif

#ifndef HAVE__STRUPR
char *MIL_strupr(char *string)
{
    char *bufp = string;
    while ( *bufp ) {
        *bufp = MIL_toupper((unsigned char) *bufp);
	++bufp;
    }
    return string;
}
#endif

#ifndef HAVE__STRLWR
char *MIL_strlwr(char *string)
{
    char *bufp = string;
    while ( *bufp ) {
        *bufp = MIL_tolower((unsigned char) *bufp);
	++bufp;
    }
    return string;
}
#endif

#ifndef HAVE_STRCHR
char *MIL_strchr(const char *string, int c)
{
    while ( *string ) {
        if ( *string == c ) {
            return (char *)string;
        }
	++string;
    }
    return NULL;
}
#endif

#ifndef HAVE_STRRCHR
char *MIL_strrchr(const char *string, int c)
{
    const char *bufp = string + MIL_strlen(string) - 1;
    while ( bufp >= string ) {
        if ( *bufp == c ) {
            return (char *)bufp;
        }
	--bufp;
    }
    return NULL;
}
#endif

#ifndef HAVE_STRSTR
char *MIL_strstr(const char *haystack, const char *needle)
{
    size_t length = MIL_strlen(needle);
    while ( *haystack ) {
        if ( MIL_strncmp(haystack, needle, length) == 0 ) {
            return (char *)haystack;
        }
	++haystack;
    }
    return NULL;
}
#endif

#if !defined(HAVE__LTOA)  || !defined(HAVE__I64TOA) || \
    !defined(HAVE__ULTOA) || !defined(HAVE__UI64TOA)
static const char ntoa_table[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};
#endif /* ntoa() conversion table */

#ifndef HAVE__LTOA
char *MIL_ltoa(long value, char *string, int radix)
{
    char *bufp = string;

    if ( value < 0 ) {
        *bufp++ = '-';
        value = -value;
    }
    if ( value ) {
        while ( value > 0 ) {
            *bufp++ = ntoa_table[value % radix];
            value /= radix;
        }
    } else {
        *bufp++ = '0';
    }
    *bufp = '\0';

    /* The numbers went into the string backwards. :) */
    if ( *string == '-' ) {
        MIL_strrev(string+1);
    } else {
        MIL_strrev(string);
    }

    return string;
}
#endif

#ifndef HAVE__ULTOA
char *MIL_ultoa(unsigned long value, char *string, int radix)
{
    char *bufp = string;

    if ( value ) {
        while ( value > 0 ) {
            *bufp++ = ntoa_table[value % radix];
            value /= radix;
        }
    } else {
        *bufp++ = '0';
    }
    *bufp = '\0';

    /* The numbers went into the string backwards. :) */
    MIL_strrev(string);

    return string;
}
#endif

#ifndef HAVE_STRTOL
long MIL_strtol(const char *string, char **endp, int base)
{
    size_t len;
    long value;

    if ( !base ) {
        if ( (MIL_strlen(string) > 2) && (MIL_strncmp(string, "0x", 2) == 0) ) {
            base = 16;
        } else {
            base = 10;
        }
    }

    len = MIL_ScanLong(string, base, &value);
    if ( endp ) {
        *endp = (char *)string + len;
    }
    return value;
}
#endif

#ifndef HAVE_STRTOUL
unsigned long MIL_strtoul(const char *string, char **endp, int base)
{
    size_t len;
    unsigned long value;

    if ( !base ) {
        if ( (MIL_strlen(string) > 2) && (MIL_strncmp(string, "0x", 2) == 0) ) {
            base = 16;
        } else {
            base = 10;
        }
    }

    len = MIL_ScanUnsignedLong(string, base, &value);
    if ( endp ) {
        *endp = (char *)string + len;
    }
    return value;
}
#endif

#ifdef MIL_HAS_64BIT_TYPE

#ifndef HAVE__I64TOA
char *MIL_lltoa(Sint64 value, char *string, int radix)
{
    char *bufp = string;

    if ( value < 0 ) {
        *bufp++ = '-';
        value = -value;
    }
    if ( value ) {
        while ( value > 0 ) {
            *bufp++ = ntoa_table[value % radix];
            value /= radix;
        }
    } else {
        *bufp++ = '0';
    }
    *bufp = '\0';

    /* The numbers went into the string backwards. :) */
    if ( *string == '-' ) {
        MIL_strrev(string+1);
    } else {
        MIL_strrev(string);
    }

    return string;
}
#endif

#ifndef HAVE__UI64TOA
char *MIL_ulltoa(Uint64 value, char *string, int radix)
{
    char *bufp = string;

    if ( value ) {
        while ( value > 0 ) {
            *bufp++ = ntoa_table[value % radix];
            value /= radix;
        }
    } else {
        *bufp++ = '0';
    }
    *bufp = '\0';

    /* The numbers went into the string backwards. :) */
    MIL_strrev(string);

    return string;
}
#endif

#ifndef HAVE_STRTOLL
Sint64 MIL_strtoll(const char *string, char **endp, int base)
{
    size_t len;
    Sint64 value;

    if ( !base ) {
        if ( (MIL_strlen(string) > 2) && (MIL_strncmp(string, "0x", 2) == 0) ) {
            base = 16;
        } else {
            base = 10;
        }
    }

    len = MIL_ScanLongLong(string, base, &value);
    if ( endp ) {
        *endp = (char *)string + len;
    }
    return value;
}
#endif

#ifndef HAVE_STRTOULL
Uint64 MIL_strtoull(const char *string, char **endp, int base)
{
    size_t len;
    Uint64 value;

    if ( !base ) {
        if ( (MIL_strlen(string) > 2) && (MIL_strncmp(string, "0x", 2) == 0) ) {
            base = 16;
        } else {
            base = 10;
        }
    }

    len = MIL_ScanUnsignedLongLong(string, base, &value);
    if ( endp ) {
        *endp = (char *)string + len;
    }
    return value;
}
#endif

#endif /* MIL_HAS_64BIT_TYPE */

#ifndef HAVE_STRTOD
double MIL_strtod(const char *string, char **endp)
{
    size_t len;
    double value;

    len = MIL_ScanFloat(string, &value);
    if ( endp ) {
        *endp = (char *)string + len;
    }
    return value;
}
#endif

#ifndef HAVE_STRCMP
int MIL_strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2) {
        if ( *str1 != *str2 )
            break;
        ++str1;
        ++str2;
    }
    return (int)((unsigned char)*str1 - (unsigned char)*str2);
}
#endif

#ifndef HAVE_STRNCMP
int MIL_strncmp(const char *str1, const char *str2, size_t maxlen)
{
    while ( *str1 && *str2 && maxlen ) {
        if ( *str1 != *str2 )
            break;
        ++str1;
        ++str2;
        --maxlen;
    }
    if ( ! maxlen ) {
        return 0;
    }
    return (int)((unsigned char)*str1 - (unsigned char)*str2);
}
#endif

#if !defined(HAVE_STRCASECMP) && !defined(HAVE__STRICMP)
int MIL_strcasecmp(const char *str1, const char *str2)
{
    char a = 0;
    char b = 0;
    while ( *str1 && *str2 ) {
        a = MIL_tolower((unsigned char) *str1);
        b = MIL_tolower((unsigned char) *str2);
        if ( a != b )
            break;
        ++str1;
        ++str2;
    }
    return (int)((unsigned char)a - (unsigned char)b);
}
#endif

#if !defined(HAVE_STRNCASECMP) && !defined(HAVE__STRNICMP)
int MIL_strncasecmp(const char *str1, const char *str2, size_t maxlen)
{
    char a = 0;
    char b = 0;
    while ( *str1 && *str2 && maxlen ) {
        a = MIL_tolower((unsigned char) *str1);
        b = MIL_tolower((unsigned char) *str2);
        if ( a != b )
            break;
        ++str1;
        ++str2;
        --maxlen;
    }
    return (int)((unsigned char)a - (unsigned char)b);
}
#endif

#ifndef HAVE_SSCANF
int MIL_sscanf(const char *text, const char *fmt, ...)
{
    va_list ap;
    int retval = 0;

    va_start(ap, fmt);
    while ( *fmt ) {
        if ( *fmt == ' ' ) {
            while ( MIL_isspace((unsigned char) *text) ) {
                ++text;
            }
            ++fmt;
            continue;
        }
        if ( *fmt == '%' ) {
            MIL_Bool done = MIL_FALSE;
            long count = 0;
            int radix = 10;
            enum {
                DO_SHORT,
                DO_INT,
                DO_LONG,
                DO_LONGLONG
            } inttype = DO_INT;
            MIL_Bool suppress = MIL_FALSE;

            ++fmt;
            if ( *fmt == '%' ) {
                if ( *text == '%' ) {
                    ++text;
                    ++fmt;
                    continue;
                }
                break;
            }
            if ( *fmt == '*' ) {
                suppress = MIL_TRUE;
                ++fmt;
            }
            fmt += MIL_ScanLong(fmt, 10, &count);

            if ( *fmt == 'c' ) {
                if ( ! count ) {
                    count = 1;
                }
                if ( suppress ) {
                    while ( count-- ) {
                        ++text;
                    }
                } else {
                    char *valuep = va_arg(ap, char*);
                    while ( count-- ) {
                        *valuep++ = *text++;
                    }
                    ++retval;
                }
                continue;
            }

            while ( MIL_isspace((unsigned char) *text) ) {
                ++text;
            }

            /* FIXME: implement more of the format specifiers */
            while (!done) {
                switch(*fmt) {
                    case '*':
                        suppress = MIL_TRUE;
                        break;
                    case 'h':
                        if ( inttype > DO_SHORT ) {
                            ++inttype;
                        }
                        break;
                    case 'l':
                        if ( inttype < DO_LONGLONG ) {
                            ++inttype;
                        }
                        break;
                    case 'I':
                        if ( MIL_strncmp(fmt, "I64", 3) == 0 ) {
                            fmt += 2;
                            inttype = DO_LONGLONG;
                        }
                        break;
                    case 'i':
                        {
                            int index = 0;
                            if ( text[index] == '-' ) {
                                ++index;
                            }
                            if ( text[index] == '0' ) {
                                if ( MIL_tolower((unsigned char) text[index+1]) == 'x' ) {
                                    radix = 16;
                                } else {
                                    radix = 8;
                                }
                            }
                        }
                        /* Fall through to %d handling */
                    case 'd':
#ifdef MIL_HAS_64BIT_TYPE
                        if ( inttype == DO_LONGLONG ) {
                            Sint64 value;
                            text += MIL_ScanLongLong(text, radix, &value);
                            if ( ! suppress ) {
                                Sint64 *valuep = va_arg(ap, Sint64*);
                                *valuep = value;
                                ++retval;
                            }
                        }
                        else
#endif /* MIL_HAS_64BIT_TYPE */
                        {
                            long value;
                            text += MIL_ScanLong(text, radix, &value);
                            if ( ! suppress ) {
                                switch (inttype) {
                                    case DO_SHORT:
                                        { short* valuep = va_arg(ap, short*);
                                            *valuep = (short)value;
                                        }
                                        break;
                                    case DO_INT:
                                        { int* valuep = va_arg(ap, int*);
                                            *valuep = (int)value;
                                        }
                                        break;
                                    case DO_LONG:
                                        { long* valuep = va_arg(ap, long*);
                                            *valuep = value;
                                        }
                                        break;
                                    case DO_LONGLONG:
                                        /* Handled above */
                                        break;
                                }
                                ++retval;
                            }
                        }
                        done = MIL_TRUE;
                        break;
                    case 'o':
                        if ( radix == 10 ) {
                            radix = 8;
                        }
                        /* Fall through to unsigned handling */
                    case 'x':
                    case 'X':
                        if ( radix == 10 ) {
                            radix = 16;
                        }
                        /* Fall through to unsigned handling */
                    case 'u':
#ifdef MIL_HAS_64BIT_TYPE
                        if ( inttype == DO_LONGLONG ) {
                            Uint64 value;
                            text += MIL_ScanUnsignedLongLong(text, radix, &value);
                            if ( ! suppress ) {
                                Uint64 *valuep = va_arg(ap, Uint64*);
                                *valuep = value;
                                ++retval;
                            }
                        }
                        else
#endif /* MIL_HAS_64BIT_TYPE */
                        {
                            unsigned long value;
                            text += MIL_ScanUnsignedLong(text, radix, &value);
                            if ( ! suppress ) {
                                switch (inttype) {
                                    case DO_SHORT:
                                        { short* valuep = va_arg(ap, short*);
                                            *valuep = (short)value;
                                        }
                                        break;
                                    case DO_INT:
                                        { int* valuep = va_arg(ap, int*);
                                            *valuep = (int)value;
                                        }
                                        break;
                                    case DO_LONG:
                                        { long* valuep = va_arg(ap, long*);
                                            *valuep = value;
                                        }
                                        break;
                                    case DO_LONGLONG:
                                        /* Handled above */
                                        break;
                                }
                                ++retval;
                            }
                        }
                        done = MIL_TRUE;
                        break;
                    case 'p':
                        {
                            uintptr_t value;
                            text += MIL_ScanUintPtrT(text, 16, &value);
                            if ( ! suppress ) {
                                void** valuep = va_arg(ap, void**);
                                *valuep = (void*)value;
                                ++retval;
                            }
                        }
                        done = MIL_TRUE;
                        break;
                    case 'f':
                        {
                            double value;
                            text += MIL_ScanFloat(text, &value);
                            if ( ! suppress ) {
                                float* valuep = va_arg(ap, float*);
                                *valuep = (float)value;
                                ++retval;
                            }
                        }
                        done = MIL_TRUE;
                        break;
                    case 's':
                        if ( suppress ) {
                            while ( !MIL_isspace((unsigned char) *text) ) {
                                ++text;
                                if ( count ) {
                                    if ( --count == 0 ) {
                                        break;
                                    }
                                }
                            }
                        } else {
                            char *valuep = va_arg(ap, char*);
                            while ( !MIL_isspace((unsigned char) *text) ) {
                                *valuep++ = *text++;
                                if ( count ) {
                                    if ( --count == 0 ) {
                                        break;
                                    }
                                }
                            }
                            *valuep = '\0';
                            ++retval;
                        }
                        done = MIL_TRUE;
                        break;
                    default:
                        done = MIL_TRUE;
                        break;
                }
                ++fmt;
            }
            continue;
        }
        if ( *text == *fmt ) {
            ++text;
            ++fmt;
            continue;
        }
        /* Text didn't match format specifier */
        break;
    }
    va_end(ap);

    return retval;
}
#endif

#ifndef HAVE_SNPRINTF
int MIL_snprintf(char *text, size_t maxlen, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = MIL_vsnprintf(text, maxlen, fmt, ap);
    va_end(ap);

    return retval;
}
#endif

#ifndef HAVE_VSNPRINTF
static size_t MIL_PrintLong(char *text, long value, int radix, size_t maxlen)
{
    char num[130];
    size_t size;

    MIL_ltoa(value, num, radix);
    size = MIL_strlen(num);
    if ( size >= maxlen ) {
        size = maxlen-1;
    }
    MIL_strlcpy(text, num, size+1);

    return size;
}
static size_t MIL_PrintUnsignedLong(char *text, unsigned long value, int radix, size_t maxlen)
{
    char num[130];
    size_t size;

    MIL_ultoa(value, num, radix);
    size = MIL_strlen(num);
    if ( size >= maxlen ) {
        size = maxlen-1;
    }
    MIL_strlcpy(text, num, size+1);

    return size;
}
#ifdef MIL_HAS_64BIT_TYPE
static size_t MIL_PrintLongLong(char *text, Sint64 value, int radix, size_t maxlen)
{
    char num[130];
    size_t size;

    MIL_lltoa(value, num, radix);
    size = MIL_strlen(num);
    if ( size >= maxlen ) {
        size = maxlen-1;
    }
    MIL_strlcpy(text, num, size+1);

    return size;
}
static size_t MIL_PrintUnsignedLongLong(char *text, Uint64 value, int radix, size_t maxlen)
{
    char num[130];
    size_t size;

    MIL_ulltoa(value, num, radix);
    size = MIL_strlen(num);
    if ( size >= maxlen ) {
        size = maxlen-1;
    }
    MIL_strlcpy(text, num, size+1);

    return size;
}
#endif /* MIL_HAS_64BIT_TYPE */
static size_t MIL_PrintFloat(char *text, double arg, size_t maxlen)
{
    char *textstart = text;
    if ( arg ) {
        /* This isn't especially accurate, but hey, it's easy. :) */
        const double precision = 0.00000001;
        size_t len;
        unsigned long value;

        if ( arg < 0 ) {
            *text++ = '-';
            --maxlen;
            arg = -arg;
        }
        value = (unsigned long)arg;
        len = MIL_PrintUnsignedLong(text, value, 10, maxlen);
        text += len;
        maxlen -= len;
        arg -= value;
        if ( arg > precision && maxlen ) {
            int mult = 10;
            *text++ = '.';
            while ( (arg > precision) && maxlen ) {
                value = (unsigned long)(arg * mult);
                len = MIL_PrintUnsignedLong(text, value, 10, maxlen);
                text += len;
                maxlen -= len;
                arg -= (double)value / mult;
                mult *= 10;
            }
        }
    } else {
        *text++ = '0';
    }
    return (text - textstart);
}
static size_t MIL_PrintString(char *text, const char *string, size_t maxlen)
{
    char *textstart = text;
    while ( *string && maxlen-- ) {
        *text++ = *string++;
    }
    return (text - textstart);
}
int MIL_vsnprintf(char *text, size_t maxlen, const char *fmt, va_list ap)
{
    char *textstart = text;
    if ( maxlen <= 0 ) {
        return 0;
    }
    --maxlen; /* For the trailing '\0' */
    while ( *fmt && maxlen ) {
        if ( *fmt == '%' ) {
            MIL_Bool done = MIL_FALSE;
            size_t len = 0;
            MIL_Bool do_lowercase = MIL_FALSE;
            int radix = 10;
            enum {
                DO_INT,
                DO_LONG,
                DO_LONGLONG
            } inttype = DO_INT;

            ++fmt;
            /* FIXME: implement more of the format specifiers */
            while ( *fmt == '.' || (*fmt >= '0' && *fmt <= '9') ) {
                ++fmt;
            }
            while (!done) {
                switch(*fmt) {
                    case '%':
                        *text = '%';
                        len = 1;
                        done = MIL_TRUE;
                        break;
                    case 'c':
                        /* char is promoted to int when passed through (...) */
                        *text = (char)va_arg(ap, int);
                        len = 1;
                        done = MIL_TRUE;
                        break;
                    case 'h':
                        /* short is promoted to int when passed through (...) */
                        break;
                    case 'l':
                        if ( inttype < DO_LONGLONG ) {
                            ++inttype;
                        }
                        break;
                    case 'I':
                        if ( MIL_strncmp(fmt, "I64", 3) == 0 ) {
                            fmt += 2;
                            inttype = DO_LONGLONG;
                        }
                        break;
                    case 'i':
                    case 'd':
                        switch (inttype) {
                            case DO_INT:
                                len = MIL_PrintLong(text, (long)va_arg(ap, int), radix, maxlen);
                                break;
                            case DO_LONG:
                                len = MIL_PrintLong(text, va_arg(ap, long), radix, maxlen);
                                break;
                            case DO_LONGLONG:
#ifdef MIL_HAS_64BIT_TYPE
                                len = MIL_PrintLongLong(text, va_arg(ap, Sint64), radix, maxlen);
#else
                                len = MIL_PrintLong(text, va_arg(ap, long), radix, maxlen);
#endif
                                break;
                        }
                        done = MIL_TRUE;
                        break;
                    case 'p':
                    case 'x':
                        do_lowercase = MIL_TRUE;
                        /* Fall through to 'X' handling */
                    case 'X':
                        if ( radix == 10 ) {
                            radix = 16;
                        }
                        if ( *fmt == 'p' ) {
                            inttype = DO_LONG;
                        }
                        /* Fall through to unsigned handling */
                    case 'o':
                        if ( radix == 10 ) {
                            radix = 8;
                        }
                        /* Fall through to unsigned handling */
                    case 'u':
                        switch (inttype) {
                            case DO_INT:
                                len = MIL_PrintUnsignedLong(text, (unsigned long)va_arg(ap, unsigned int), radix, maxlen);
                                break;
                            case DO_LONG:
                                len = MIL_PrintUnsignedLong(text, va_arg(ap, unsigned long), radix, maxlen);
                                break;
                            case DO_LONGLONG:
#ifdef MIL_HAS_64BIT_TYPE
                                len = MIL_PrintUnsignedLongLong(text, va_arg(ap, Uint64), radix, maxlen);
#else
                                len = MIL_PrintUnsignedLong(text, va_arg(ap, unsigned long), radix, maxlen);
#endif
                                break;
                        }
                        if ( do_lowercase ) {
                            MIL_strlwr(text);
                        }
                        done = MIL_TRUE;
                        break;
                    case 'f':
                        len = MIL_PrintFloat(text, va_arg(ap, double), maxlen);
                        done = MIL_TRUE;
                        break;
                    case 's':
                        len = MIL_PrintString(text, va_arg(ap, char*), maxlen);
                        done = MIL_TRUE;
                        break;
                    default:
                        done = MIL_TRUE;
                        break;
                }
                ++fmt;
            }
            text += len;
            maxlen -= len;
        } else {
            *text++ = *fmt++;
            --maxlen;
        }
    }
    *text = '\0';

    return (text - textstart);
}
#endif
