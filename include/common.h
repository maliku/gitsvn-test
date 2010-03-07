/*!============================================================================
 * @file types.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月07日 20时30分12秒 
 *  Organization: http://www.ds0101.net
 */


#ifndef  TYPES_INC
#define  TYPES_INC

/* The number of elements in a table */
#define MIL_TABLESIZE(table)	(sizeof(table)/sizeof(table[0]))

/* Basic data types */
typedef enum {
	MIL_FALSE = 0,
	MIL_TRUE  = 1
} MIL_bool;

typedef unsigned char	Uint8;
typedef signed char	Sint8;
typedef unsigned short	Uint16;
typedef signed short	Sint16;
typedef unsigned int	Uint32;
typedef signed int	Sint32;

/* Figure out how to support 64-bit datatypes */
#if !defined(__STRICT_ANSI__)
#ifdef __osf__ /* Tru64 */
#define MIL_HAS_64BIT_TYPE	long
#elif defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_C) || defined(__DECC)
#define MIL_HAS_64BIT_TYPE	long long
#elif defined(_MSC_VER) /* VC++ */
#define MIL_HAS_64BIT_TYPE	__int64
#endif
#endif /* !__STRICT_ANSI__ */

/* The 64-bit type isn't available on EPOC/Symbian OS */
#ifdef __SYMBIAN32__
#undef MIL_HAS_64BIT_TYPE
#endif

/* The 64-bit datatype isn't supported on all platforms */
#ifdef MIL_HAS_64BIT_TYPE
#ifndef H_MMBASIC
typedef unsigned MIL_HAS_64BIT_TYPE Uint64;
#endif
typedef MIL_HAS_64BIT_TYPE Sint64;
#else
/* This is really just a hack to prevent the compiler from complaining */
typedef struct {
	Uint32 hi;
	Uint32 lo;
} Uint64, Sint64;
#endif

/* Make sure the types really have the right sizes */
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

/* Check to make sure enums are the size of ints, for structure packing.
   For both Watcom C/C++ and Borland C/C++ the compiler option that makes
   enums having the size of an int must be enabled.
   This is "-b" for Borland C/C++ and "-ei" for Watcom C/C++ (v11).
*/
/* Enable enums always int in CodeWarrior (for MPW use "-enum int") */
#ifdef __MWERKS__
#pragma enumsalwaysint on
#endif

typedef enum {
	DUMMY_ENUM_VALUE
} MIL_DUMMY_ENUM;

MIL_COMPILE_TIME_ASSERT(enum, sizeof(MIL_DUMMY_ENUM) == sizeof(int));

#undef MIL_COMPILE_TIME_ASSERT

/* General keyboard/mouse state definitions */
enum { MIL_PRESSED = 0x01, MIL_RELEASED = 0x00 };


#endif   /* ----- #ifndef TYPES_INC  ----- */
