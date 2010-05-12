/*
    MIL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file provides a general interface for MIL to read and write
   data sources.  It can easily be extended to files, memory, etc.
*/

#ifndef _MIL_RWops_h
#define _MIL_RWops_h

#include <stdio.h>

#include "coo.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This is the read/write operation structure -- very basic */

CLASS(MIL_RWops) 
{
    VIRTUAL_METHOD_DECLARE_BEGIN(MIL_RWops)
        /* Seek to 'offset' relative to whence, one of stdio's whence values:
           SEEK_SET, SEEK_CUR, SEEK_END
           Returns the final offset in the data source.
           */
        int (MILCALL *seek)(_Self(MIL_RWops), int offset, int whence);

    /* Read up to 'num' objects each of size 'objsize' from the data
       source to the area pointed at by 'ptr'.
       Returns the number of objects read, or -1 if the read failed.
       */
    int (MILCALL *read)(_Self(MIL_RWops), void *ptr, int size, int maxnum);

    /* Write exactly 'num' objects each of size 'objsize' from the area
       pointed at by 'ptr' to data source.
       Returns 'num', or -1 if the write failed.
       */
    int (MILCALL *write)(_Self(MIL_RWops), const void *ptr, int size, int num);

    /* Close and free an allocated MIL_FSops structure */
    int (MILCALL *close)(_Self(MIL_RWops));

    VIRTUAL_METHOD_DECLARE_END

    METHOD_DECLARE_PLACEHOLDER(MIL_RWops)
    Uint32 type;
    union {
        struct {
            int autoclose;
            FILE *fp;
        } stdio;
        struct {
            Uint8 *base;
            Uint8 *here;
            Uint8 *stop;
        } mem;
        struct {
            void *data1;
        } unknown;
    } hidden;

};

CLASS_INHERIT_BEGIN(RawFileOperator, MIL_RWops)
    NO_VIRTUAL_METHOD_EXPAND(RawFileOperator)
    METHOD_DECLARE_PLACEHOLDER(RawFileOperator)
CLASS_INHERIT_END 

CLASS_INHERIT_BEGIN(MemFileOperator, MIL_RWops)
    NO_VIRTUAL_METHOD_EXPAND(MemFileOperator)
    METHOD_DECLARE_PLACEHOLDER(MemFileOperator)
CLASS_INHERIT_END 

/* Functions to create MIL_RWops structures from various data sources */

extern DECLSPEC MIL_RWops * MILCALL MIL_RWFromFile(const char *file, const char *mode);

extern DECLSPEC MIL_RWops * MILCALL MIL_RWFromFP(FILE *fp, int autoclose);

extern DECLSPEC MIL_RWops * MILCALL MIL_RWFromMem(void *mem, int size);
extern DECLSPEC MIL_RWops * MILCALL MIL_RWFromConstMem(const void *mem, int size);

/* extern DECLSPEC MIL_RWops * MILCALL MIL_AllocRW(void);
 * extern DECLSPEC void MILCALL MIL_FreeRW(MIL_RWops *area);
 */

/** @name Seek Reference Points */
/*@{*/
#define RW_SEEK_SET	0	/**< Seek from the beginning of data */
#define RW_SEEK_CUR	1	/**< Seek relative to current read point */
#define RW_SEEK_END	2	/**< Seek relative to the end of data */
/*@}*/


/* Macros to easily read and write from an MIL_RWops structure */
#define MIL_RWseek(ctx, offset, whence)	_VC(ctx)->seek(ctx, offset, whence)
#define MIL_RWtell(ctx)			_VC(ctx)->seek(ctx, 0, SEEK_CUR)
#define MIL_RWread(ctx, ptr, size, n)	_VC(ctx)->read(ctx, ptr, size, n)
#define MIL_RWwrite(ctx, ptr, size, n)	_VC(ctx)->write(ctx, ptr, size, n)
#define MIL_RWclose(ctx)		_VC(ctx)->close(ctx)


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _MIL_RWops_h */
