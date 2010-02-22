/*!============================================================================
 * @file coo_common.h 
 * @Synopsis Common file of C with Object-Orienthed 
 * @author DongKai
 * @version 1.0
 * @date 2010年02月05日 23时14分22秒 
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef __COO_COMMON_H__
#define __COO_COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef TYPE_OF
#undef TYPE_OF
#endif
#define TYPE_OF(val) __typeof__(val)

#define FPOS( type, field ) \
    ( (int) &(( type *) 0)->field )

#define FSIZ( type, field ) sizeof( ((type *) 0)->field )

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#ifdef False
#undef False
#endif
#define False 0

#ifdef True
#undef True
#endif
#define True (!False)

#ifdef Bool
#undef Bool
#endif
#define Bool int

#ifdef INLINE
#undef INLINE
#endif
#define INLINE inline

#endif
