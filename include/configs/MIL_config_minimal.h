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

#ifndef _MIL_config_minimal_h
#define _MIL_config_minimal_h

#include "MIL_platform.h"

/* This is the minimal configuration that can be used to build MIL */

#include <stdarg.h>

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef unsigned int size_t;
typedef unsigned long uintptr_t;

/* Enable the dummy audio driver (src/audio/dummy/\*.c) */
#define MIL_AUDIO_DRIVER_DUMMY	1

/* Enable the stub cdrom driver (src/cdrom/dummy/\*.c) */
#define MIL_CDROM_DISABLED	1

/* Enable the stub joystick driver (src/joystick/dummy/\*.c) */
#define MIL_JOYSTICK_DISABLED	1

/* Enable the stub shared object loader (src/loadso/dummy/\*.c) */
#define MIL_LOADSO_DISABLED	1

/* Enable the stub thread support (src/thread/generic/\*.c) */
#define MIL_THREADS_DISABLED	1

/* Enable the stub timer support (src/timer/dummy/\*.c) */
#define MIL_TIMERS_DISABLED	1

/* Enable the dummy video driver (src/video/dummy/\*.c) */
#define MIL_VIDEO_DRIVER_DUMMY	1

#endif /* _MIL_config_minimal_h */
