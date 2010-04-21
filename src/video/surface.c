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

void METHOD_NAMED(Surface, setClipRect)(_Self(MIL_Surface), const MIL_Rect *rect)
{}

void METHOD_NAMED(Surface, getClipRect)(_Self(MIL_Surface), MIL_Rect *rect)
{}

int  METHOD_NAMED(Surface, blitSurface)(_Self(MIL_Surface), MIL_Rect *srcrect, MIL_Surface *dst, MIL_Rect *dstrect)
{}

int  METHOD_NAMED(Surface, fillRect)(_Self(MIL_Surface), MIL_Rect *dstrect, Uint32 color)
{}

int  METHOD_NAMED(Surface, saveBMP)(_Self(MIL_Surface), const char *file)
{}

MIL_Surface* METHOD_NAMED(Surface, displayFormat)(_Self(MIL_Surface))
{}

MIL_Surface* METHOD_NAMED(Surface, displayFormatAlpha)(_Self(MIL_Surface))
{}

MIL_Surface* METHOD_NAMED(Surface, convert)(_Self(MIL_Surface), MIL_PixelFormat *fmt, Uint32 flags)
{
    return NULL;
}

Uint32 METHOD_NAMED(Surface, getWidth)(_CSelf(MIL_Surface))
{
    return ((const Surface*)self)->w;
}

Uint32 METHOD_NAMED(Surface, getHeight)(_CSelf(MIL_Surface))
{
    return ((const Surface*)self)->h;
}

Uint32 METHOD_NAMED(Surface, getPitch)(_CSelf(MIL_Surface))
{
    return ((const Surface*)self)->pitch;
}

Uint32 METHOD_NAMED(Surface, getFlags)(_CSelf(MIL_Surface))
{
    return ((const Surface*)self)->flags;
}

void METHOD_NAMED(Surface, setWidth)(_Self(MIL_Surface), Uint32 w)
{
    ((Surface*)self)->w = w;
}

void METHOD_NAMED(Surface, setHeight)(_Self(MIL_Surface), Uint32 h)
{
    ((Surface*)self)->h = h;
}

void METHOD_NAMED(Surface, setPitch)(_Self(MIL_Surface), Uint32 pitch)
{
    ((Surface*)self)->pitch = pitch;
}

void METHOD_NAMED(Surface, setFlags)(_Self(MIL_Surface), Uint32 flags)
{
    ((Surface*)self)->flags = flags;
}

VIRTUAL_METHOD_REGBEGIN(Surface, MIL_Surface)
NON_DESTRUCTOR
METHOD_REGISTER(Surface, lock)
METHOD_REGISTER(Surface, unlock)
METHOD_REGISTER(Surface, setColorKey)
METHOD_REGISTER(Surface, setAlpha)
METHOD_REGISTER(Surface, setClipRect)
METHOD_REGISTER(Surface, getClipRect)
METHOD_REGISTER(Surface, blitSurface)
METHOD_REGISTER(Surface, fillRect)
METHOD_REGISTER(Surface, saveBMP)
METHOD_REGISTER(Surface, displayFormat)
METHOD_REGISTER(Surface, displayFormatAlpha)
METHOD_REGISTER(Surface, convert)

METHOD_REGISTER(Surface, getWidth)
METHOD_REGISTER(Surface, getHeight)
METHOD_REGISTER(Surface, getPitch)
METHOD_REGISTER(Surface, getFlags)

METHOD_REGISTER(Surface, setWidth)
METHOD_REGISTER(Surface, setHeight)
METHOD_REGISTER(Surface, setPitch)
METHOD_REGISTER(Surface, setFlags)

VIRTUAL_METHOD_REGEND 

METHOD_REGISTER_PLACEHOLDER(Surface)
