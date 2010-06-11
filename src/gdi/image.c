/*! ============================================================================
 * @file image.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "misc.h"
#include "image.h"

CONSTRUCTOR(MIL_Image)
{
    _private(MIL_Image)->data = NULL;
    _private(MIL_Image)->raw_format = NULL;
    _private(MIL_Image)->cache = NULL;
    return self;
}

DESTRUCTOR(MIL_Image)
{
}

MIL_Image*  MIL_Image_X_clone(_Self(MIL_Image))
{
    return NULL;
}

MIL_Status  MIL_Image_X_getBounds(_Self(MIL_Image), MIL_Rect* rc)
{
    if (NULL != rc) {
        rc->x = rc->y = 0;
        rc->w = _private(MIL_Image)->data->w;
        rc->h = _private(MIL_Image)->data->h;
    }
    return MIL_NOT_IMPLEMENTED;
}

Uint32      MIL_Image_X_getWidth(_Self(MIL_Image))
{
    return _private(MIL_Image)->data->w;
}

Uint32      MIL_Image_X_getHeight(_Self(MIL_Image))
{
    return _private(MIL_Image)->data->h;
}

MIL_Status  MIL_Image_X_getPalette(_Self(MIL_Image), MIL_Palette* palette)
{
    if (NULL != palette) {
        // TODO: should be check _private(MIL_Image)->data->format.
        return MIL_GENERIC_ERROR;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status  MIL_Image_X_setPalette(_Self(MIL_Image), const MIL_Palette* palette)
{
    if (NULL != palette) {
        return MIL_GENERIC_ERROR;
    }
    return MIL_INVALID_PARAMETER;
}

int MIL_Image_X_getPaletteSize(_Self(MIL_Image))
{
    MIL_PixelFormat* fmt = _private(MIL_Image)->data->format;
    if (NULL != fmt) {
        return _c(fmt)->getPaletteSize(fmt);
    }
    return 0;
}

const MIL_PixelFormat* METHOD_NAMED(MIL_Image, getPixelFormat)
    (_Self(MIL_Image))
{
    return _private(MIL_Image)->data->format;
}

const char* MIL_Image_X_getRawFormat(_Self(MIL_Image))
{
    return _private(MIL_Image)->raw_format;
}

MIL_Status  MIL_Image_X_rotateFlip(_Self(MIL_Image), MIL_RotateFlipType type)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_Image_X_save(_Self(MIL_Status), const char* file)
{
    if (NULL != file) {
        MIL_RWops* ops = MIL_RWFromFile(file, "wb");
        if (NULL != ops) {
            return MIL_NOT_IMPLEMENTED;
//            return MIL_SaveBMP_RW(((Surface*)_private(MIL_Image)->surface), ops, MIL_AUTO_FREE);
        }
        return MIL_OUT_OF_MEMORY;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(MIL_Image, loadFile)(_Self(MIL_Image), const char* file)
{
    if (NULL != file) {
        MIL_RWops* ops = NULL;
        // TODO: add metafile support.
        if (NULL == _private(MIL_Image)->cache) {
            if (NULL != _private(MIL_Image)->data) {
                MIL_free(_private(MIL_Image)->data);
                _private(MIL_Image)->data = NULL;
            }
        }
        else {
            _private(MIL_Image)->data = NULL;
            Delete((Surface*)_private(MIL_Image)->cache);
        }

        ops = MIL_RWFromFile(file, "rb");
        if (NULL != ops) {
            Surface* surface = MIL_LoadBMP_RW(ops, MIL_AUTO_FREE);
            if (NULL != surface) {
                _private(MIL_Image)->data = (MIL_BitmapData*)(&surface->format);
                _private(MIL_Image)->raw_format = "bmp";
                _private(MIL_Image)->cache = surface;
                return MIL_OK;
            }
            else {
                return MIL_OUT_OF_MEMORY;
            }
        }
        else {
            return MIL_FILE_NOT_FOUND;
        }
    }
    return MIL_INVALID_PARAMETER;
}

BEGIN_METHOD_MAP(MIL_Image, MIL_GdiObject)
    CONSTRUCTOR_MAP(MIL_Image)
    DESTRUCTOR_MAP(MIL_Image)
    METHOD_MAP(MIL_Image, clone)
    METHOD_MAP(MIL_Image, getBounds)
    METHOD_MAP(MIL_Image, getWidth)
    METHOD_MAP(MIL_Image, getHeight)
    METHOD_MAP(MIL_Image, getPalette)
    METHOD_MAP(MIL_Image, setPalette)
    METHOD_MAP(MIL_Image, getPaletteSize)
    METHOD_MAP(MIL_Image, getPixelFormat)
    METHOD_MAP(MIL_Image, getRawFormat)
    METHOD_MAP(MIL_Image, rotateFlip)
    METHOD_MAP(MIL_Image, save)
    METHOD_MAP(MIL_Image, loadFile)
END_METHOD_MAP

/* Tmpfunc: Watch the lifecycle of surface. */
MIL_Image*
CreateImageFromSurface(Surface* surface)
{
    if (NULL != surface) {
        MIL_Image* img = (MIL_Image*)New(MIL_Image);
        if (NULL != img) {
            _friend(MIL_Image, img)->data = (MIL_BitmapData*)(&surface->format);
            _friend(MIL_Image, img)->raw_format = "surface";
            _friend(MIL_Image, img)->cache = surface;

            return img;
        }
    }
    return NULL;
}


MIL_Image* LoadImageFromFile(const char* file)
{
    if (NULL != file) {
        MIL_Image* img = (MIL_Image*)New(MIL_Image);
        if (NULL != img) {
            if (MIL_OK == _c(img)->loadFile(img, file)) {
                return (MIL_Image*)img;
            }
            else {
                Delete(img);
            }
        }
    }

    return NULL;
}

