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

/* Useful functions and variables from MIL_RLEaccel.c */

//extern int MIL_RLESurface(Surface *surface);
extern int MIL_RLEBlit(Surface *src, MIL_Rect *srcrect,
                       Surface *dst, MIL_Rect *dstrect);
extern int MIL_RLEAlphaBlit(Surface *src, MIL_Rect *srcrect,
			    Surface *dst, MIL_Rect *dstrect);
//extern void MIL_UnRLESurface(Surface *surface, int recode);
extern int RLEAlphaSurface(Surface *surface);
extern int RLEColorkeySurface(Surface *surface);
extern MIL_Bool UnRLEAlpha(Surface *surface);
