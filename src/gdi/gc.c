/*! ============================================================================
 * @file gfx_context.c 
 * @Synopsis Implementation for graphics context. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "MIL_endian.h"
#include "misc.h"
#include "surface.h"
#include "gc.h"
#include "pixels_ops.h"
#include "shape.h"

METHOD_MAP_PLACEHOLDER(MIL_GraphicsContext, MIL_GdiObject)

CONSTRUCTOR(MemoryGC)
{
    _m(image) = NULL;
    _m(pen) = NULL;
    _m(brush) = NULL;
    return self;
}

DESTRUCTOR(MemoryGC)
{
    MIL_GdiObject* img_obj = (MIL_GdiObject*)(_m(image));
    MIL_GdiObject* pen_obj = (MIL_GdiObject*)(_m(pen));
    MIL_GdiObject* brush_obj = (MIL_GdiObject*)(_m(brush));
    if (img_obj) {
        _c(img_obj)->unRef(img_obj, MIL_HOLD_REF);
    }
    if (pen_obj) {
        _c(pen_obj)->unRef(pen_obj, MIL_HOLD_REF);
    }
    if (brush_obj) {
        _c(brush_obj)->unRef(brush_obj, MIL_HOLD_REF);
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
            _c(_tm(MemoryGC, image))->getPixelFormat(_tm(MemoryGC, image)); 
        Surface* surface = _friend(MIL_Image, _tm(MemoryGC, image))->cache;
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
        if (NULL != _tm(MemoryGC, pen)) {
            _c(_tm(MemoryGC, pen))->unRef(_tm(MemoryGC, pen), MIL_HOLD_REF);
        }
        _c((MIL_GdiObject*)pen)->ref((MIL_GdiObject*)pen, MIL_HOLD_REF);
        _tm(MemoryGC, pen) = pen;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MemoryGC, selectBrush)(_Self(MIL_GraphicsContext), MIL_Brush* brush)
{
    if (NULL != brush) {
        if (NULL != _tm(MemoryGC, brush)) {
            _c(_tm(MemoryGC, brush))->unRef(_tm(MemoryGC, brush), MIL_HOLD_REF);
        }
        _c((MIL_GdiObject*)brush)->ref((MIL_GdiObject*)brush, MIL_HOLD_REF);
        _tm(MemoryGC, brush) = brush;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MemoryGC, selectPixelsOperator)
    (_Self(MIL_GraphicsContext))
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status METHOD_NAMED(MemoryGC, setPixelOperationType)
    (_Self(MIL_GraphicsContext), MIL_PixelsOperation opt)
{
    /* TODO: compare operaion wheather eqaul*/
    if (opt >= 0 && opt < MIL_PIXEL_OPERATION_MAX) {
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MemoryGC, setPixel)(_Self(MIL_GraphicsContext), Uint32 x, Uint32 y)
{
    MIL_Color c;
    const MIL_PixelFormat* fmt = 
        _c(_tm(MemoryGC, image))->getPixelFormat(_tm(MemoryGC, image));
    // TODO: clipping.
    _tm(MemoryGC, pix_ctxt).cur_dst = 
        (Uint8*)_friend(MIL_Image, _tm(MemoryGC, image))->data->scan0 +
                (y * _friend(MIL_Image, _tm(MemoryGC, image))->data->pitch + 
                 x * _c(fmt)->getBytesPerPixel(fmt));
    _c(_tm(MemoryGC, pen))->getColor(_tm(MemoryGC, pen), &c);
    _tm(MemoryGC, pix_ctxt).color = 
        _c(fmt)->mapColor(fmt, &c);
    SetPixelSet16(&_tm(MemoryGC, pix_ctxt));
    return MIL_OK;
}

mt_color  METHOD_NAMED(MemoryGC, getPixel)(_Self(MIL_GraphicsContext), Uint32 x, Uint32 y)
{
    const MIL_PixelFormat* fmt = 
        _c(_tm(MemoryGC, image))->getPixelFormat(_tm(MemoryGC, image));
    Uint8* dst = 
        (Uint8*)_friend(MIL_Image, _tm(MemoryGC, image))->data->scan0 +
                (y * _friend(MIL_Image, _tm(MemoryGC, image))->data->pitch + 
                 x * _c(fmt)->getBytesPerPixel(fmt));

    switch (_c(fmt)->getBytesPerPixel(fmt)) {
        case 1:
            return *dst;
        case 2:
            return *(Uint16*)dst;
        case 3:
            {
                mt_color pixel;
                if (MIL_BYTEORDER == MIL_LIL_ENDIAN) {
                    pixel = dst[0] + (dst[1] << 8) + (dst[2] << 16);
                } else {
                    pixel = (dst[0] << 16) + (dst[1] << 8) + dst[2];
                }
                return pixel;
            }
        case 4:
            return *(Uint32*)dst;
    }

    return 0;
}

MIL_Status METHOD_NAMED(MemoryGC, drawLine)
    (_Self(MIL_GraphicsContext), Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2)
{
    LineGenerator(self, x1, y1, x2, y2, METHOD_NAMED(MemoryGC, setPixel));
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
    METHOD_MAP(MemoryGC, selectPixelsOperator)
    METHOD_MAP(MemoryGC, setPixel)
    METHOD_MAP(MemoryGC, getPixel)
    METHOD_MAP(MemoryGC, drawLine)
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
            const MIL_PixelFormat* fmt = _c(img)->getPixelFormat(img);
            if (_c((MIL_GdiObject*)img)->getRef(img, MIL_HOLD_REF) <= 0) {
                mgc->image = 
                    (MIL_Image*)_c((MIL_GdiObject*)img)->ref(img, MIL_HOLD_REF);
//                _c(mgc)->selectPixelsOperator((MIL_GraphicsContext*)mgc, 
//                        GetPixelsOperator(MIL_PIXEL_COPY, _c(fmt)->getBytesPerPixel(fmt)));
                return (MIL_Graphics*)mgc;
            }
        }
    }
    return NULL;
}

