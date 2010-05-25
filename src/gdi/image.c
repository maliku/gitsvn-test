/*! ============================================================================
 * @file image.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "image.h"

METHOD_MAP_PLACEHOLDER(MIL_GdiObject, NonBase)

CONSTRUCTOR(MIL_Image)
{
    _private(MIL_Image)->data = NULL;
    _private(MIL_Image)->raw_format = NULL;
    return self;
}

DESTRUCTOR(MIL_Image)
{
    Delete(_private(MIL_Image)->data);
}

MIL_Image*  MIL_Image_X_clone(_Self(MIL_Image))
{
    return NULL;
}

MIL_Status  MIL_Image_X_getBounds(_Self(MIL_Image), MIL_Rect* rc)
{
    if (NULL != rc) {
        Surface* surface = _private(MIL_Image)->data;
        rc->x = rc->y = 0;
        rc->w = _c(surface)->getWidth(surface);
        rc->h = _c(surface)->getHeight(surface);
    }
    return MIL_NOT_IMPLEMENTED;
}

Uint32      MIL_Image_X_getWidth(_Self(MIL_Image))
{
    Surface* surface = _private(MIL_Image)->data;
    return _c(surface)->getWidth(surface);
}

Uint32      MIL_Image_X_getHeight(_Self(MIL_Image))
{
    Surface* surface = _private(MIL_Image)->data;
    return _c(surface)->getHeight(surface);
}

MIL_Status  MIL_Image_X_getPalette(_Self(MIL_Image), MIL_Palette* palette)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_Image_X_setPalette(_Self(MIL_Image), const MIL_Palette* palette)
{
    return MIL_NOT_IMPLEMENTED;
}

int         MIL_Image_X_getPaletteSize(_Self(MIL_Image))
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_Image_X_getPixelFormat(_Self(MIL_Image), MIL_PixelFormat* fmt)
{
    if (NULL != fmt) {
        Surface* surface = _private(MIL_Image)->data;
        memcpy(fmt, surface->format, sizeof(*fmt));
    }
    return MIL_INVALID_PARAMETER;
}

const char* MIL_Image_X_getRawFormat(_Self(MIL_Image))
{
    return NULL;
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

MIL_Status MIL_Image_X_loadFile(_Self(MIL_Image), const char* file)
{
    if (NULL != file) {
        MIL_RWops* ops;
        if (NULL != _private(MIL_Image)->data) {
            Delete(_private(MIL_Image)->data);
            _private(MIL_Image)->data = NULL;
        }

        ops = MIL_RWFromFile(file, "rb");
        if (NULL != ops) {
            Surface* surface = MIL_LoadBMP_RW(ops, MIL_AUTO_FREE);
            if (NULL != surface) {
                _private(MIL_Image)->data = surface;
                _private(MIL_Image)->raw_format = "bmp";
                printf("format=%p, pixels=%p.\n", surface->format, surface->pixels);
                if ( surface->map != NULL ) {
                    MIL_FreeBlitMap(surface->map);
                    surface->map = NULL;
                }
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

METHOD_MAP_BEGIN(MIL_Image, MIL_GdiObject)
    CONSTRUCTOR_MAP(MIL_Image)
    DESTRUCTOR_MAP(MIL_Image)
    METHOD_PLACEHOLDER(addHoldRef)
    METHOD_PLACEHOLDER(decHoldRef)
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
METHOD_MAP_END




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

