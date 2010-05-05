/*
    MIL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

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

    Sam Lantinga
    slouken@libsdl.org
*/
#include "MIL_config.h"

/* Useful functions and variables from MIL_pixel.c */

#include "blit.h"

/* Pixel format functions */
//MIL_PixelFormat* MIL_ReallocFormat(Surface *surface, int bpp,
//		Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
//extern void MIL_FormatChanged(Surface *surface);
//extern void MIL_FreeFormat(MIL_PixelFormat *format);

/* Blit mapping functions */
extern MIL_BlitMap* MIL_AllocBlitMap(void);
extern void MIL_InvalidateMap(MIL_BlitMap *map);
//extern int MIL_MapSurface (Surface *src, Surface *dst);
extern void MIL_FreeBlitMap(MIL_BlitMap *map);

/* Miscellaneous functions */
//extern Uint16 MIL_CalculatePitch(Surface *surface);
extern void MIL_DitherColors(MIL_Color *colors, int bpp);
extern Uint8 MIL_FindColor(MIL_Palette *pal, Uint8 r, Uint8 g, Uint8 b);
extern void MIL_ApplyGamma(Uint16 *gamma, MIL_Color *colors, MIL_Color *output, int ncolors);
