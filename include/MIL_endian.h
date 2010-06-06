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

/**
 *  @file MIL_endian.h
 *  Functions for reading and writing endian-specific values
 */

#ifndef _MIL_endian_h
#define _MIL_endian_h

#include "MIL_stdinc.h"

/** @name MIL_ENDIANs
 *  The two types of endianness 
 */
/*@{*/
#define MIL_LIL_ENDIAN	1234
#define MIL_BIG_ENDIAN	4321
/*@}*/

#ifndef MIL_BYTEORDER	/* Not defined in MIL_config.h? */
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define MIL_BYTEORDER	MIL_BIG_ENDIAN
#else
#define MIL_BYTEORDER	MIL_LIL_ENDIAN
#endif
#endif /* !MIL_BYTEORDER */


#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @name MIL_Swap Functions
 *  Use inline functions for compilers that support them, and static
 *  functions for those that do not.  Because these functions become
 *  static for compilers that do not support inline functions, this
 *  header should only be included in files that actually use them.
 */
/*@{*/
#if defined(__GNUC__) && defined(__i386__) && \
   !(__GNUC__ == 2 && __GNUC_MINOR__ <= 95 /* broken gcc version */)
static __INLINE__ Uint16 MIL_Swap16(Uint16 x)
{
	__asm__("xchgb %b0,%h0" : "=q" (x) :  "0" (x));
	return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __INLINE__ Uint16 MIL_Swap16(Uint16 x)
{
	__asm__("xchgb %b0,%h0" : "=Q" (x) :  "0" (x));
	return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
static __INLINE__ Uint16 MIL_Swap16(Uint16 x)
{
	Uint16 result;

	__asm__("rlwimi %0,%2,8,16,23" : "=&r" (result) : "0" (x >> 8), "r" (x));
	return result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__))
static __INLINE__ Uint16 MIL_Swap16(Uint16 x)
{
	__asm__("rorw #8,%0" : "=d" (x) :  "0" (x) : "cc");
	return x;
}
#else
static __INLINE__ Uint16 MIL_Swap16(Uint16 x) {
	return((x<<8)|(x>>8));
}
#endif

#if defined(__GNUC__) && defined(__i386__) && \
   !(__GNUC__ == 2 && __GNUC_MINOR__ <= 95 /* broken gcc version */)
static __INLINE__ Uint32 MIL_Swap32(Uint32 x)
{
	__asm__("bswap %0" : "=r" (x) : "0" (x));
	return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __INLINE__ Uint32 MIL_Swap32(Uint32 x)
{
	__asm__("bswapl %0" : "=r" (x) : "0" (x));
	return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
static __INLINE__ Uint32 MIL_Swap32(Uint32 x)
{
	Uint32 result;

	__asm__("rlwimi %0,%2,24,16,23" : "=&r" (result) : "0" (x>>24), "r" (x));
	__asm__("rlwimi %0,%2,8,8,15"   : "=&r" (result) : "0" (result),    "r" (x));
	__asm__("rlwimi %0,%2,24,0,7"   : "=&r" (result) : "0" (result),    "r" (x));
	return result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__))
static __INLINE__ Uint32 MIL_Swap32(Uint32 x)
{
	__asm__("rorw #8,%0\n\tswap %0\n\trorw #8,%0" : "=d" (x) :  "0" (x) : "cc");
	return x;
}
#else
static __INLINE__ Uint32 MIL_Swap32(Uint32 x) {
	return((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
}
#endif

#ifdef MIL_HAS_64BIT_TYPE
#if defined(__GNUC__) && defined(__i386__) && \
   !(__GNUC__ == 2 && __GNUC_MINOR__ <= 95 /* broken gcc version */)
static __INLINE__ Uint64 MIL_Swap64(Uint64 x)
{
	union { 
		struct { Uint32 a,b; } s;
		Uint64 u;
	} v;
	v.u = x;
	__asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1" 
	        : "=r" (v.s.a), "=r" (v.s.b) 
	        : "0" (v.s.a), "1" (v.s.b)); 
	return v.u;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __INLINE__ Uint64 MIL_Swap64(Uint64 x)
{
	__asm__("bswapq %0" : "=r" (x) : "0" (x));
	return x;
}
#else
static __INLINE__ Uint64 MIL_Swap64(Uint64 x)
{
	Uint32 hi, lo;

	/* Separate into high and low 32-bit values and swap them */
	lo = MIL_static_cast(Uint32, x & 0xFFFFFFFF);
	x >>= 32;
	hi = MIL_static_cast(Uint32, x & 0xFFFFFFFF);
	x = MIL_Swap32(lo);
	x <<= 32;
	x |= MIL_Swap32(hi);
	return(x);
}
#endif
#else
/* This is mainly to keep compilers from complaining in MIL code.
 * If there is no real 64-bit datatype, then compilers will complain about
 * the fake 64-bit datatype that MIL provides when it compiles user code.
 */
#define MIL_Swap64(X)	(X)
#endif /* MIL_HAS_64BIT_TYPE */
/*@}*/

/**
 *  @name MIL_SwapLE and MIL_SwapBE Functions
 *  Byteswap item from the specified endianness to the native endianness
 */
/*@{*/
#if MIL_BYTEORDER == MIL_LIL_ENDIAN
#define MIL_SwapLE16(X)	(X)
#define MIL_SwapLE32(X)	(X)
#define MIL_SwapLE64(X)	(X)
#define MIL_SwapBE16(X)	MIL_Swap16(X)
#define MIL_SwapBE32(X)	MIL_Swap32(X)
#define MIL_SwapBE64(X)	MIL_Swap64(X)

#define EQUAL_24BIT(ptr, val)  \
    ((*(Uint8*)(ptr)      == ((Uint8)(val))) \
     &&(*((Uint8*)(ptr)+1) == ((Uint8)((val)>>8))) \
     &&(*((Uint8*)(ptr)+2) == ((Uint8)((val)>>16))))

#define SETVAL_24BIT(ptr, val) do { \
    *(Uint8*)(ptr)     = (Uint8)(val); \
    *((Uint8*)(ptr)+1) = (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) = (Uint8)((val)>>16); \
}while(0)

#define ORVAL_24BIT(ptr, val)  do { \
    *(Uint8*)(ptr)     |= (Uint8)(val); \
    *((Uint8*)(ptr)+1) |= (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) |= (Uint8)((val)>>16);  \
}while(0)

#define ANDVAL_24BIT(ptr, val) do { \
    *(Uint8*)(ptr)     &= (Uint8)(val); \
    *((Uint8*)(ptr)+1) &= (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) &= (Uint8)((val)>>16);  \
}while(0)

#define XORVAL_24BIT(ptr, val) do { \
    *(Uint8*)(ptr)     ^= (Uint8)(val); \
    *((Uint8*)(ptr)+1) ^= (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) ^= (Uint8)((val)>>16); \
}while(0)

#define READPTR_24BIT(val, ptr) ((val) = (*(Uint8*)ptr) \
        + ((*((Uint8*)(ptr)+1))<<8) + ((*((Uint8*)(ptr)+2))<<16))

#else
#define MIL_SwapLE16(X)	MIL_Swap16(X)
#define MIL_SwapLE32(X)	MIL_Swap32(X)
#define MIL_SwapLE64(X)	MIL_Swap64(X)
#define MIL_SwapBE16(X)	(X)
#define MIL_SwapBE32(X)	(X)
#define MIL_SwapBE64(X)	(X)

#define EQUAL_24BIT(ptr, val)  \
    ((*(Uint8*)(ptr)      == ((Uint8)((val)>>16))) \
     &&(*((Uint8*)(ptr)+1) == ((Uint8)((val)>>8))) \
     &&(*((Uint8*)(ptr)+2) == ((Uint8)((val)))))

#define SETVAL_24BIT(ptr, val) do { \
    *(Uint8*)(ptr)     = (Uint8)((val)>>16); \
    *((Uint8*)(ptr)+1) = (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) = (Uint8)((val));  \
}while(0)

#define ORVAL_24BIT(ptr, val)  do { \
    *(Uint8*)(ptr)     |= (Uint8)((val)>>16); \
    *((Uint8*)(ptr)+1) |= (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) |= (Uint8)((val));  \
}while(0)

#define ANDVAL_24BIT(ptr, val)do { \
    *(Uint8*)(ptr)     &= (Uint8)((val)>>16); \
    *((Uint8*)(ptr)+1) &= (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) &= (Uint8)((val));  \
}while(0)

#define XORVAL_24BIT(ptr, val) do { \
    *(Uint8*)(ptr)     ^= (Uint8)((val)>>16); \
    *((Uint8*)(ptr)+1) ^= (Uint8)((val)>>8); \
    *((Uint8*)(ptr)+2) ^= (Uint8)((val)); \
}while(0)

#define READPTR_24BIT(val, ptr) ((val) = (((*(Uint8*)ptr)<<16) \
            + ((*((Uint8*)(ptr)+1))<<8) + (*((Uint8*)(ptr)+2))))

#endif
/*@}*/

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _MIL_endian_h */
