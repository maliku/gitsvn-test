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
#include "pixels_ops.h"

METHOD_MAP_PLACEHOLDER(MIL_GraphicsContext, MIL_GdiObject)

CONSTRUCTOR(MemoryGC)
{
    _private(MemoryGC)->image = NULL;
    _private(MemoryGC)->pen = NULL;
    _private(MemoryGC)->brush = NULL;
    _private(MemoryGC)->pixels_ops = NULL;
    return self;
}

DESTRUCTOR(MemoryGC)
{
    MIL_GdiObject* img_obj = (MIL_GdiObject*)(_private(MemoryGC)->image);
    MIL_GdiObject* pen_obj = (MIL_GdiObject*)(_private(MemoryGC)->pen);
    MIL_GdiObject* brush_obj = (MIL_GdiObject*)(_private(MemoryGC)->brush);
    MIL_GdiObject* ops_obj = (MIL_GdiObject*)(_private(MemoryGC)->pixels_ops);
    if (img_obj) {
        _c(img_obj)->unRef(img_obj, MIL_HOLD_REF);
    }
    if (pen_obj) {
        _c(pen_obj)->unRef(pen_obj, MIL_HOLD_REF);
    }
    if (brush_obj) {
        _c(brush_obj)->unRef(brush_obj, MIL_HOLD_REF);
    }
    if (ops_obj) {
        _c(ops_obj)->unRef(ops_obj, MIL_HOLD_REF);
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
        Surface* surface = _friend(MIL_Image, _private(MemoryGC)->image)->cache;
        _c(surface)->getClipRect(surface, &rc_bak);
        _c(surface)->setClipRect(surface, NULL);
        _c(surface)->fillRect(surface, NULL, _c(format)->mapColor(format, color));
        _c(surface)->setClipRect(surface, &rc_bak);
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
    Surface* src = (_friend(MIL_Image, img)->cache);
}

MIL_Status METHOD_NAMED(MemoryGC, drawImageByRect)
    (_Self(MIL_GraphicsContext), MIL_Image* img, MIL_Rect* rc)
{
    Surface* src = (_friend(MIL_Image, img)->cache);
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

MIL_Status METHOD_NAMED(MemoryGC, selectPen)(_Self(MIL_GraphicsContext), MIL_Pen* pen)
{
    if (NULL != pen) {
        _c((MIL_GdiObject*)pen)->ref((MIL_GdiObject*)pen, MIL_HOLD_REF);
        _private(MemoryGC)->pen = pen;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MemoryGC, selectBrush)(_Self(MIL_GraphicsContext), MIL_Brush* brush)
{
    if (NULL != brush) {
        _c((MIL_GdiObject*)brush)->ref((MIL_GdiObject*)brush, MIL_HOLD_REF);
        _private(MemoryGC)->brush = brush;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MemoryGC, selectPixelsOperations)(_Self(MIL_GraphicsContext), MIL_PixelsOps* ops)
{
    if (NULL != ops) {
        _c((MIL_GdiObject*)ops)->ref((MIL_GdiObject*)ops, MIL_HOLD_REF);
        _private(MemoryGC)->pixels_ops = ops;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MemoryGC, drawPixel)(_Self(MIL_GraphicsContext), int x, int y)
{
    MIL_PixelsOps* ops = _private(MemoryGC)->pixels_ops;
    _friend(MIL_PixelsOps, ops)->cur_dst = 
        (Uint8*)_friend(MIL_Image, _private(MemoryGC)->image)->data->scan0 +
                (y * _friend(MIL_Image, _private(MemoryGC)->image)->data->pitch + x * 4/* FIXME */);
    _c(ops)->setPixel(ops);
    return MIL_OK;
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
    METHOD_MAP(MemoryGC, selectPen)
    METHOD_MAP(MemoryGC, selectBrush)
    METHOD_MAP(MemoryGC, selectPixelsOperations)
    METHOD_MAP(MemoryGC, drawPixel)
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
        mgc = MIL_CreateMemGCFromImage(img);
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
MIL_CreateMemGCFromImage(MIL_Image* img)
{
    if (NULL != img) {
        MemoryGC* mgc = (MemoryGC*)New(MemoryGC);
        if (NULL != mgc) {
            if (_c((MIL_GdiObject*)img)->getRef(img, MIL_HOLD_REF) <= 0) {
                _friend(MemoryGC, mgc)->image = 
                    (MIL_Image*)_c((MIL_GdiObject*)img)->ref(img, MIL_HOLD_REF);
                // TODO: create pixelsops by depth.
                _friend(MemoryGC, mgc)->pixels_ops = (MIL_PixelsOps*)New(PixelsSet4);
                _friend(MIL_PixelsOps, _friend(MemoryGC, mgc)->pixels_ops)->cur_dst = _friend(MIL_Image, _friend(MemoryGC, mgc)->image)->data->scan0;
                    return (MIL_Graphics*)mgc;
            }
        }
    }
    return NULL;
}

