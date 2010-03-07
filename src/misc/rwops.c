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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* #include "MIL_error.h"
 */
#include "MIL_rwops.h"

#define MIL_Error(err)
/* Functions to read/write stdio file pointers */

static int stdio_seek(MIL_RWops *context, int offset, int whence)
{
	if ( fseek(context->hidden.stdio.fp, offset, whence) == 0 ) {
		return(ftell(context->hidden.stdio.fp));
	} else {
		MIL_Error(MIL_EFSEEK);
		return(-1);
	}
}
static int stdio_read(MIL_RWops *context, void *ptr, int size, int maxnum)
{
	size_t nread;

	nread = fread(ptr, size, maxnum, context->hidden.stdio.fp); 
	if ( nread == 0 && ferror(context->hidden.stdio.fp) ) {
		MIL_Error(MIL_EFREAD);
	}
	return(nread);
}
static int stdio_write(MIL_RWops *context, const void *ptr, int size, int num)
{
	size_t nwrote;

	nwrote = fwrite(ptr, size, num, context->hidden.stdio.fp);
	if ( nwrote == 0 && ferror(context->hidden.stdio.fp) ) {
		MIL_Error(MIL_EFWRITE);
	}
	return(nwrote);
}
static int stdio_close(MIL_RWops *context)
{
	if ( context ) {
		if ( context->hidden.stdio.autoclose ) {
			/* WARNING:  Check the return value here! */
			fclose(context->hidden.stdio.fp);
		}
		free(context);
	}
	return(0);
}

/* Functions to read/write memory pointers */

static int mem_seek(MIL_RWops *context, int offset, int whence)
{
	Uint8 *newpos;

	switch (whence) {
		case SEEK_SET:
			newpos = context->hidden.mem.base+offset;
			break;
		case SEEK_CUR:
			newpos = context->hidden.mem.here+offset;
			break;
		case SEEK_END:
			newpos = context->hidden.mem.stop+offset;
			break;
		default:
			MIL_SetError("Unknown value for 'whence'");
			return(-1);
	}
	if ( newpos < context->hidden.mem.base ) {
		newpos = context->hidden.mem.base;
	}
	if ( newpos > context->hidden.mem.stop ) {
		newpos = context->hidden.mem.stop;
	}
	context->hidden.mem.here = newpos;
	return(context->hidden.mem.here-context->hidden.mem.base);
}
static int mem_read(MIL_RWops *context, void *ptr, int size, int maxnum)
{
	int total_bytes;
	int mem_available;

	total_bytes = (maxnum * size);
	if ( (maxnum <= 0) || (size <= 0) || ((total_bytes / maxnum) != size) ) {
		return 0;
	}

	mem_available = (context->hidden.mem.stop - context->hidden.mem.here);
	if (total_bytes > mem_available) {
		total_bytes = mem_available;
	}

	memcpy(ptr, context->hidden.mem.here, total_bytes);
	context->hidden.mem.here += total_bytes;

	return (total_bytes / size);
}
static int mem_write(MIL_RWops *context, const void *ptr, int size, int num)
{
	if ( (context->hidden.mem.here + (num*size)) > context->hidden.mem.stop ) {
		num = (context->hidden.mem.stop-context->hidden.mem.here)/size;
	}
	memcpy(context->hidden.mem.here, ptr, num*size);
	context->hidden.mem.here += num*size;
	return(num);
}
static int mem_writeconst(MIL_RWops *context, const void *ptr, int size, int num)
{
	MIL_SetError("Can't write to read-only memory");
	return(-1);
}
static int mem_close(MIL_RWops *context)
{
	if ( context ) {
		free(context);
	}
	return(0);
}

/* Functions to create MIL_RWops structures from various data sources */
#ifdef WIN32
/* Aggh.  You can't (apparently) open a file in an application and
   read from it in a DLL.
*/
static int in_sdl = 0;
#endif

#ifdef macintosh
/*
 * translate unix-style slash-separated filename to mac-style colon-separated
 * name; return malloced string
 */
static char *unix_to_mac(const char *file)
{
	int flen = strlen(file);
	char *path = malloc(flen + 2);
	const char *src = file;
	char *dst = path;
	if(*src == '/') {
		/* really depends on filesystem layout, hope for the best */
		src++;
	} else {
		/* Check if this is a MacOS path to begin with */
		if(*src != ':')
			*dst++ = ':';   /* relative paths begin with ':' */
	}
	while(src < file + flen) {
		const char *end = strchr(src, '/');
		int len;
		if(!end)
			end = file + flen; /* last component */
		len = end - src;
		if(len == 0 || (len == 1 && src[0] == '.')) {
			/* remove repeated slashes and . */
		} else {
			if(len == 2 && src[0] == '.' && src[1] == '.') {
				/* replace .. with the empty string */
			} else {
				memcpy(dst, src, len);
				dst += len;
			}
			if(end < file + flen)
				*dst++ = ':';
		}
		src = end + 1;
	}
	*dst++ = '\0';
	return path;
}
#endif /* macintosh */

MIL_RWops *MIL_RWFromFile(const char *file, const char *mode)
{
	FILE *fp;
	MIL_RWops *rwops;

	rwops = NULL;

#ifdef macintosh
	{
		char *mpath = unix_to_mac(file);
		fp = fopen(mpath, mode);
		free(mpath);
	}
#else
	fp = fopen(file, mode);
#endif
	if ( fp == NULL ) {
		MIL_SetError("Couldn't open %s", file);
	} else {
#ifdef WIN32
		in_sdl = 1;
		rwops = MIL_RWFromFP(fp, 1);
		in_sdl = 0;
#else
		rwops = MIL_RWFromFP(fp, 1);
#endif
	}
	return(rwops);
}

MIL_RWops *MIL_RWFromFP(FILE *fp, int autoclose)
{
	MIL_RWops *rwops;

#ifdef WIN32
	if ( ! in_sdl ) {
		MIL_SetError("You can't pass a FILE pointer to a DLL (?)");
		/*return(NULL);*/
	}
#endif
	rwops = New(RawFileOperator);
	if ( rwops != NULL ) {
		rwops->hidden.stdio.fp = fp;
		rwops->hidden.stdio.autoclose = autoclose;
	}
	return(rwops);
}

MIL_RWops *MIL_RWFromMem(void *mem, int size)
{
	MIL_RWops *rwops = New(MemFileOperator);

	if ( rwops != NULL ) {
		rwops->hidden.mem.base = (Uint8 *)mem;
		rwops->hidden.mem.here = rwops->hidden.mem.base;
		rwops->hidden.mem.stop = rwops->hidden.mem.base+size;
	}
	return(rwops);
}

MIL_RWops *MIL_RWFromConstMem(const void *mem, int size)
{
	MIL_RWops *rwops = (MIL_RWops*)New(MemFileOperator);
	if ( rwops != NULL ) {
		rwops->hidden.mem.base = (Uint8 *)mem;
		rwops->hidden.mem.here = rwops->hidden.mem.base;
		rwops->hidden.mem.stop = rwops->hidden.mem.base+size;
	}
	return(rwops);
}

MIL_RWops *MIL_AllocRW(void)
{
    assert(NULL);
	MIL_RWops *area;

	area = (MIL_RWops *)malloc(sizeof *area);
	if ( area == NULL ) {
		MIL_OutOfMemory();
	}
	return(area);
}

void MIL_FreeRW(MIL_RWops *area)
{
    assert(NULL);
	free(area);
}

/* Make MIL_RWops to a pure vitrual class. */
MAKE_PURE_VIRTUAL_CLASS(MIL_RWops)
MEMBER_FUNC_REGISTER_PLACEHOLDER(MIL_RWops)

VIRTUAL_FUNCTION_REGBEGIN(RawFileOperator, MIL_RWops)
    NON_DESTRUCTOR
    stdio_seek,
    stdio_read,
    stdio_write,
    stdio_close,
VIRTUAL_FUNCTION_REGEND
MEMBER_FUNC_REGISTER_PLACEHOLDER(RawFileOperator)

VIRTUAL_FUNCTION_REGBEGIN(MemFileOperator, MIL_RWops)
    NON_DESTRUCTOR
    mem_seek,
    mem_read,
    mem_write,
    mem_close,
VIRTUAL_FUNCTION_REGEND
MEMBER_FUNC_REGISTER_PLACEHOLDER(MemFileOperator)
