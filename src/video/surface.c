/*! ============================================================================
 * @file surface.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月11日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "surface.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Surface)
METHOD_REGISTER_PLACEHOLDER(MIL_Surface)

CONSTRUCTOR(Surface)
{
    return self;
}

DESTRUCTOR(Surface)
{
}

int  METHOD_NAMED(Surface, lock)(_Self(MIL_Surface))
{}

void METHOD_NAMED(Surface, unlock)(_Self(MIL_Surface))
{}

int  METHOD_NAMED(Surface, setColorKey)(_Self(MIL_Surface), Uint32 flag, Uint32 key)
{}

int  METHOD_NAMED(Surface, setAlpha)(_Self(MIL_Surface), Uint32 flag, Uint8 alpha)
{}

void METHOD_NAMED(Surface, setClipRect)(_Self(MIL_Surface), MIL_Rect *rect)
{}

void METHOD_NAMED(Surface, getClipRect)(_Self(MIL_Surface), MIL_Rect *rect)
{}

int  METHOD_NAMED(Surface, blitSurface)(_Self(MIL_Surface), MIL_Rect *srcrect, Surface *dst, MIL_Rect *dstrect)
{}

int  METHOD_NAMED(Surface, fillRect)(_Self(MIL_Surface), MIL_Rect *dstrect, Uint32 color)
{}

int  METHOD_NAMED(Surface, saveBMP)(_Self(MIL_Surface), const char *file)
{}

Surface* METHOD_NAMED(Surface, displayFormat)(_Self(MIL_Surface))
{}

Surface* METHOD_NAMED(Surface, displayFormatAlpha)(_Self(MIL_Surface))
{}

Surface* METHOD_NAMED(Surface, convert)(_Self(MIL_Surface), MIL_PixelFormat *fmt, Uint32 flags)
{}

