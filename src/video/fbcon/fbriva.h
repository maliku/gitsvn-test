/*
    SDL - A embedded system graphics Middle Interface Layer.
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
#include "SDL_config.h"

/* Riva hardware acceleration for the SDL framebuffer console driver */

#include "SDL_fbvideo.h"

#ifndef FB_ACCEL_NV3
#define FB_ACCEL_NV3	27
#endif
#ifndef FB_ACCEL_NV4
#define FB_ACCEL_NV4	28
#endif

/* Set up the driver for Riva acceleration */
extern void FB_RivaAccel(_THIS, __u32 card);
