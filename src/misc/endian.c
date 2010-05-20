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

/* Functions for dynamically reading and writing endian-specific values */

#include "MIL_endian.h"
#include "MIL_rwops.h"

Uint16 MIL_ReadLE16 (MIL_RWops *src)
{
	Uint16 value;

	MIL_RWread(src, &value, (sizeof value), 1);
	return(MIL_SwapLE16(value));
}
Uint16 MIL_ReadBE16 (MIL_RWops *src)
{
	Uint16 value;

	MIL_RWread(src, &value, (sizeof value), 1);
	return(MIL_SwapBE16(value));
}
Uint32 MIL_ReadLE32 (MIL_RWops *src)
{
	Uint32 value;

	MIL_RWread(src, &value, (sizeof value), 1);
	return(MIL_SwapLE32(value));
}
Uint32 MIL_ReadBE32 (MIL_RWops *src)
{
	Uint32 value;

	MIL_RWread(src, &value, (sizeof value), 1);
	return(MIL_SwapBE32(value));
}
Uint64 MIL_ReadLE64 (MIL_RWops *src)
{
	Uint64 value;

	MIL_RWread(src, &value, (sizeof value), 1);
	return(MIL_SwapLE64(value));
}
Uint64 MIL_ReadBE64 (MIL_RWops *src)
{
	Uint64 value;

	MIL_RWread(src, &value, (sizeof value), 1);
	return(MIL_SwapBE64(value));
}

int MIL_WriteLE16 (MIL_RWops *dst, Uint16 value)
{
	value = MIL_SwapLE16(value);
	return(MIL_RWwrite(dst, &value, (sizeof value), 1));
}
int MIL_WriteBE16 (MIL_RWops *dst, Uint16 value)
{
	value = MIL_SwapBE16(value);
	return(MIL_RWwrite(dst, &value, (sizeof value), 1));
}
int MIL_WriteLE32 (MIL_RWops *dst, Uint32 value)
{
	value = MIL_SwapLE32(value);
	return(MIL_RWwrite(dst, &value, (sizeof value), 1));
}
int MIL_WriteBE32 (MIL_RWops *dst, Uint32 value)
{
	value = MIL_SwapBE32(value);
	return(MIL_RWwrite(dst, &value, (sizeof value), 1));
}
int MIL_WriteLE64 (MIL_RWops *dst, Uint64 value)
{
	value = MIL_SwapLE64(value);
	return(MIL_RWwrite(dst, &value, (sizeof value), 1));
}
int MIL_WriteBE64 (MIL_RWops *dst, Uint64 value)
{
	value = MIL_SwapBE64(value);
	return(MIL_RWwrite(dst, &value, (sizeof value), 1));
}
