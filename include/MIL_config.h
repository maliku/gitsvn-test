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

#ifndef _MIL_config_h
#define _MIL_config_h

#include "configs/MIL_platform.h"

/* Add any platform that doesn't build using the configure system */
#if defined(__DREAMCAST__)
#include "configs/MIL_config_dreamcast.h"
#elif defined(__MACOS__)
#include "configs/MIL_config_macos.h"
#elif defined(__MACOSX__)
#include "configs/MIL_config_macosx.h"
#elif defined(__SYMBIAN32__)
#include "configs/MIL_config_symbian.h"  /* must be before win32! */
#elif defined(__WIN32__)
#include "configs/MIL_config_win32.h"
#elif defined(__OS2__)
#include "configs/MIL_config_os2.h"
#elif defined(__LINUX__)
#include "MIL_config_linux.h"
#else
#include "configs/MIL_config_minimal.h"
#endif /* platform config */

#endif /* _MIL_config_h */
