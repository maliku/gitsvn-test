/*! ============================================================================
 * @file surface.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-11
 *  Organization: http://www.ds0101.net
 */

#include "surface.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Surface)
METHOD_REGISTER_PLACEHOLDER(MIL_Surface)

CONSTRUCTOR(Surface)
{
    self->lock = (MIL_mutex*)MIL_CreateMutex();
    return self;
}

DESTRUCTOR(Surface)
{
    MIL_DestroyMutex(_tm(Surface, lock));
}

void* Surface_X_lock(_SELF)
{
    _VC(_tm(Surface, lock))->lock(self);
    return _tm(Surface, pixels);
}

void Surface_X_unlock(_SELF)
{
    _VC(_tm(Surface, lock))->unlock(self);
}

int  Surface_X_setColorKey(_SELF, Uint32 flag, Uint32 key)
{}

int  Surface_X_setAlpha(_SELF, Uint32 flag, Uint8 alpha)
{}

void Surface_X_setClipRect(_SELF, const MIL_Rect *rect)
{}

void Surface_X_getClipRect(_SELF, MIL_Rect *rect)
{}

int  Surface_X_blitSurface(_SELF, MIL_Rect *srcrect, MIL_Surface *dst, MIL_Rect *dstrect)
{}

int  Surface_X_fillRect(_SELF, MIL_Rect *dstrect, Uint32 color)
{}

int  Surface_X_saveBMP(_SELF, const char *file)
{}

MIL_Surface* Surface_X_displayFormat(_SELF)
{}

MIL_Surface* Surface_X_displayFormatAlpha(_SELF)
{}

MIL_Surface* Surface_X_convert(_SELF, MIL_PixelFormat *fmt, Uint32 flags)
{
    return NULL;
}

Uint32 Surface_X_getWidth(_CSELF)
{
    return ((const Surface*)self)->w;
}

Uint32 Surface_X_getHeight(_CSELF)
{
    return ((const Surface*)self)->h;
}

Uint32 Surface_X_getPitch(_CSELF)
{
    return ((const Surface*)self)->pitch;
}

Uint32 Surface_X_getFlags(_CSELF)
{
    return ((const Surface*)self)->flags;
}

Uint8 Surface_X_getBitsPerPixel(_CSELF)
{
    return ((const Surface*)self)->format->BitsPerPixel;
}

Uint8 Surface_X_getBytesPerPixel(_CSELF)
{
    return ((const Surface*)self)->format->BytesPerPixel;
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
METHOD_REGISTER(Surface, getBitsPerPixel)
METHOD_REGISTER(Surface, getBytesPerPixel)

VIRTUAL_METHOD_REGEND 

METHOD_REGISTER_PLACEHOLDER(Surface)
