/*! ============================================================================
 * @file gfx_context.c 
 * @Synopsis Implementation for graphics context. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "misc.h"
#include "surface.h"
#include "gc.h"

METHOD_MAP_PLACEHOLDER(MIL_GraphicsContext, MIL_GdiObject)

CONSTRUCTOR(MemoryGC)
{
    _private(MemoryGC)->image = NULL;
    _private(MemoryGC)->pen = NULL;
    _private(MemoryGC)->brush = NULL;
    return self;
}

DESTRUCTOR(MemoryGC)
{
    MIL_GdiObject* img = (MIL_GdiObject*)(_private(MemoryGC)->image);
    if (img) {
        _c(img)->unRef(img, HOLD_REF);
    }
}

/** 
 * @name clear
 * @synopsis clears a MemoryGC object to a specified color. Not affected by
 * Clipping region.
 * 
 * @param self
 * @param color Pointer to the Color object that specifies the color to paint 
 * the background.
 * 
 * @returns If the method succeeds, it returns MIL_OK. 
 */
MIL_Status METHOD_NAMED(MemoryGC, clear)(_Self(MIL_GraphicsContext), MIL_Color* color)
{
    if (NULL != color) {
        MIL_Rect rc_bak;
        const MIL_PixelFormat* format =
            _c(_private(MemoryGC)->image)->getPixelFormat(
                    _private(MemoryGC)->image); 
        Surface* surface = _friend(MIL_Image, _private(MemoryGC)->image)->data;
        _c(surface)->fillRect(surface, NULL, _c(format)->mapColor(format, color));
        return MIL_OK;
    }
    return MIL_INVALID_PARAMETER;
}

/** 
 * @synopsis  
 * 
 * @param MIL_GraphicsContext
 * @param img
 * @param x
 * @param y
 * 
 * @returns   
 */
MIL_Status METHOD_NAMED(MemoryGC, drawImageByPos)
    (_Self(MIL_GraphicsContext), MIL_Image* img, int x, int y)
{
    Surface* src = (_friend(MIL_Image, img)->data);
}

MIL_Status METHOD_NAMED(MemoryGC, drawImageByRect)
    (_Self(MIL_GraphicsContext), MIL_Image* img, MIL_Rect* rc)
{

}

MIL_Status METHOD_NAMED(MemoryGC, drawString)
    (_Self(MIL_GraphicsContext), const char* str, MIL_Rect* rc)
{

}

MIL_Status METHOD_NAMED(MemoryGC, includeClip)
    (_Self(MIL_GraphicsContext), MIL_Rect* rc)
{
    
}

MIL_Status METHOD_NAMED(MemoryGC, excludeClip)
    (_Self(MIL_GraphicsContext), MIL_Rect* rc)
{

}

MIL_Bool   METHOD_NAMED(MemoryGC, isClipEmpty)
    (_Self(MIL_GraphicsContext))
{

}

void METHOD_NAMED(MemoryGC, store)(_Self(MIL_GraphicsContext))
{

}

void METHOD_NAMED(MemoryGC, restore)(_Self(MIL_GraphicsContext))
{

}

BEGIN_METHOD_MAP(MemoryGC, MIL_GraphicsContext)
    CONSTRUCTOR_MAP(MemoryGC)
    DESTRUCTOR_MAP(MemoryGC)
    METHOD_MAP(MemoryGC, clear)
    METHOD_MAP(MemoryGC, drawImageByPos)
    METHOD_MAP(MemoryGC, drawImageByRect)
    METHOD_MAP(MemoryGC, drawString)
    METHOD_MAP(MemoryGC, includeClip)
    METHOD_MAP(MemoryGC, excludeClip)
    METHOD_MAP(MemoryGC, isClipEmpty)
    METHOD_MAP(MemoryGC, store)
    METHOD_MAP(MemoryGC, restore)
END_METHOD_MAP


MIL_Graphics*
MIL_CreateMemGC(Uint32 width, Uint32 height, Uint32 depth, Uint32 Rmask,
        Uint32 Gmask, Uint32 Bmask, Uint32 Amask, Uint32 flags)
{
    Surface* surface = CreateRGBSurface (flags, width, height, depth,
            Rmask, Gmask, Bmask, Amask);
    MIL_Image* img = (MIL_Image*)CreateImageFromSurface(surface);
    MIL_Graphics* mgc = NULL;
    if (NULL != img) {
        mgc = CreateMemGCFromImage(img);
        if (NULL == mgc) {
            Delete(img); /* It will delete surface too. */
        }
    }
    else {
        Delete(surface);
    }
    return mgc;
}

MIL_Graphics*
CreateMemGCFromImage(MIL_Image* img)
{
    if (NULL != img) {
        MemoryGC* mgc = (MemoryGC*)New(MemoryGC);
        if (NULL != mgc) {
            if (_c((MIL_GdiObject*)img)->getRef(img, HOLD_REF) <= 0) {
                _friend(MemoryGC, mgc)->image = 
                    (MIL_Image*)_c((MIL_GdiObject*)img)->ref(img, HOLD_REF);
                return (MIL_Graphics*)mgc;
            }
        }
    }
    return NULL;
}

