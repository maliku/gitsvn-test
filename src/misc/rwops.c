/*
    MIL - A embedded system graphics Middle Interface Layer.
    Copyright (C) 1997-2004 Dong Kai

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

    Dong Kai
    libmilui@gmail.com
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

/* Functions to read/write stdio file pointers */

int METHOD_NAMED(RawFileOperator, seek)(_Self(MIL_RWops), int offset, int whence)
{
	if (fseek(self->hidden.stdio.fp, offset, whence) == 0 ) {
		return(ftell(self->hidden.stdio.fp));
	} else {
		MIL_Error(MIL_EFSEEK);
		return(-1);
	}
}

int METHOD_NAMED(RawFileOperator, read)(_Self(MIL_RWops), void *ptr, int size, int maxnum)
{
	size_t nread;

	nread = fread(ptr, size, maxnum, self->hidden.stdio.fp); 
	if ( nread == 0 && ferror(self->hidden.stdio.fp) ) {
		MIL_Error(MIL_EFREAD);
	}
	return(nread);
}

int METHOD_NAMED(RawFileOperator, write)(_Self(MIL_RWops), const void *ptr, int size, int num)
{
	size_t nwrote;
	nwrote = fwrite(ptr, size, num, self->hidden.stdio.fp);
	if ( nwrote == 0 && ferror(self->hidden.stdio.fp) ) {
        perror("ohh");
		MIL_Error(MIL_EFWRITE);
	}
	return(nwrote);
}

int METHOD_NAMED(RawFileOperator, close)(_Self(MIL_RWops))
{
	if ( self ) {
		if ( self->hidden.stdio.autoclose ) {
			/* WARNING:  Check the return value here! */
			fclose(self->hidden.stdio.fp);
		}
		free(self);
	}
	return(0);
}

/* Functions to read/write memory pointers */

int METHOD_NAMED(MemFileOperator, seek)(_Self(MIL_RWops), int offset, int whence)
{
	Uint8 *newpos;

	switch (whence) {
		case SEEK_SET:
			newpos = self->hidden.mem.base+offset;
			break;
		case SEEK_CUR:
			newpos = self->hidden.mem.here+offset;
			break;
		case SEEK_END:
			newpos = self->hidden.mem.stop+offset;
			break;
		default:
			//MIL_SetError("Unknown value for 'whence'");
			return(-1);
	}
	if ( newpos < self->hidden.mem.base ) {
		newpos = self->hidden.mem.base;
	}
	if ( newpos > self->hidden.mem.stop ) {
		newpos = self->hidden.mem.stop;
	}
	self->hidden.mem.here = newpos;
	return(self->hidden.mem.here-self->hidden.mem.base);
}
int METHOD_NAMED(MemFileOperator, read)(_Self(MIL_RWops), void *ptr, int size, int maxnum)
{
	int total_bytes;
	int mem_available;

	total_bytes = (maxnum * size);
	if ( (maxnum <= 0) || (size <= 0) || ((total_bytes / maxnum) != size) ) {
		return 0;
	}

	mem_available = (self->hidden.mem.stop - self->hidden.mem.here);
	if (total_bytes > mem_available) {
		total_bytes = mem_available;
	}

	memcpy(ptr, self->hidden.mem.here, total_bytes);
	self->hidden.mem.here += total_bytes;

	return (total_bytes / size);
}
int METHOD_NAMED(MemFileOperator, write)(_Self(MIL_RWops), const void *ptr, int size, int num)
{
	if ( (self->hidden.mem.here + (num*size)) > self->hidden.mem.stop ) {
		num = (self->hidden.mem.stop-self->hidden.mem.here)/size;
	}
	memcpy(self->hidden.mem.here, ptr, num*size);
	self->hidden.mem.here += num*size;
	return(num);
}
int mem_writeconst(_Self(MIL_RWops), const void *ptr, int size, int num)
{
	//MIL_SetError("Can't write to read-only memory");
	return(-1);
}
int METHOD_NAMED(MemFileOperator, close)(_Self(MIL_RWops))
{
	if ( self ) {
		free(self);
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
		//MIL_SetError("Couldn't open %s", file);
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
	MIL_RWops *rwops = NULL;

#ifdef WIN32
	if ( ! in_sdl ) {
		//MIL_SetError("You can't pass a FILE pointer to a DLL (?)");
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

/* MIL_RWops *MIL_RWFromConstMem(const void *mem, int size)
 * {
 * 	MIL_RWops *rwops = (MIL_RWops*)New(MemFileOperator);
 * 	if ( rwops != NULL ) {
 * 		rwops->hidden.mem.base = (Uint8 *)mem;
 * 		rwops->hidden.mem.here = rwops->hidden.mem.base;
 * 		rwops->hidden.mem.stop = rwops->hidden.mem.base+size;
 * 	}
 * 	return(rwops);
 * }
 */

/* MIL_RWops *MIL_AllocRW(void)
 * {
 *     assert(NULL);
 * 	MIL_RWops *area;
 * 
 * 	area = (MIL_RWops *)malloc(sizeof *area);
 * 	if ( area == NULL ) {
 * 		MIL_OutOfMemory();
 * 	}
 * 	return(area);
 * }
 * 
 * void MIL_FreeRW(MIL_RWops *area)
 * {
 *     assert(NULL);
 * 	free(area);
 * }
 */

/* Make MIL_RWops to a pure vitrual class. */
MAKE_PURE_VIRTUAL_CLASS(MIL_RWops)

METHOD_MAP_BEGIN(RawFileOperator, MIL_RWops)
    NON_DESTRUCTOR
    METHOD_MAP(RawFileOperator, seek)
    METHOD_MAP(RawFileOperator, read)
    METHOD_MAP(RawFileOperator, write)
    METHOD_MAP(RawFileOperator, close)
METHOD_MAP_END

METHOD_MAP_BEGIN(MemFileOperator, MIL_RWops)
    NON_DESTRUCTOR
    METHOD_MAP(MemFileOperator, seek)
    METHOD_MAP(MemFileOperator, read)
    METHOD_MAP(MemFileOperator, write)
    METHOD_MAP(MemFileOperator, close)
METHOD_MAP_END

