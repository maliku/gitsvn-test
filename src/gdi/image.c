/*! ============================================================================
 * @file image.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "image.h"

VIRTUAL_METHOD_REGISTER_PLACEHOLDER(MIL_Image, NonBase)
METHOD_REGISTER_PLACEHOLDER(MIL_Image)

CONSTRUCTOR(Image)
{
    _private(MIL_Image)->surface = NULL;
    return self;
}

DESTRUCTOR(Image)
{
    Delete(_private(MIL_Image)->surface);
}

MIL_Image*  Image_X_clone(_Self(MIL_Image))
{
    return NULL;
}

MIL_Status  Image_X_getBounds(_Self(MIL_Image), MIL_Rect* rc)
{
    if (NULL != rc) {
        rc->x = rc->y = 0;
        rc->w = _VC(self)->getWidth(self);
        rc->h = _VC(self)->getHeight(self);
    }
    return MIL_NOT_IMPLEMENTED;
}

Uint32      Image_X_getWidth(_Self(MIL_Image))
{
    return ((Surface*)_private(MIL_Image)->surface)->w;
}

Uint32      Image_X_getHeight(_Self(MIL_Image))
{
    return ((Surface*)_private(MIL_Image)->surface)->h;
}

MIL_Status  Image_X_getPalette(_Self(MIL_Image), MIL_Palette* palette)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  Image_X_setPalette(_Self(MIL_Image), const MIL_Palette* palette)
{
    return MIL_NOT_IMPLEMENTED;
}

int         Image_X_getPaletteSize(_Self(MIL_Image))
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  Image_X_getPixelFormat(_Self(MIL_Image), MIL_PixelFormat* fmt)
{
    if (NULL != fmt) {
        memcpy(fmt, ((Surface*)_private(MIL_Image)->surface)->format, sizeof(*fmt));
    }
    return MIL_INVALID_PARAMETER;
}

const char* Image_X_getRawFormat(_Self(MIL_Image))
{
    return NULL;
}

MIL_Status  Image_X_rotateFlip(_Self(MIL_Image), MIL_RotateFlipType type)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  Image_X_save(_Self(MIL_Status), const char* file)
{
    if (NULL != file) {
        MIL_RWops* ops = MIL_RWFromFile(file, "wb");
        if (NULL != ops) {
            return MIL_SaveBMP_RW(((Surface*)_private(MIL_Image)->surface), ops, MIL_AUTO_FREE);
        }
        return MIL_OUT_OF_MEMORY;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status Image_X_loadFile(_SELF, const char* file)
{
    if (NULL != file) {
        MIL_RWops* ops;
        if (NULL != _private(MIL_Image)->surface) {
            Delete(_private(MIL_Image)->surface);
            _private(MIL_Image)->surface = NULL;
        }
        ops = MIL_RWFromFile(file, "rb");
        if (NULL != ops) {
            _private(MIL_Image)->surface = MIL_LoadBMP_RW(ops, MIL_AUTO_FREE);
            return NULL != _private(MIL_Image)->surface ? MIL_OK : MIL_OUT_OF_MEMORY;
        }
        else {
            return MIL_FILE_NOT_FOUND;
        }
    }
    return MIL_INVALID_PARAMETER;
}

VIRTUAL_METHOD_REGBEGIN(Image, MIL_Image)
    DESTRUCTOR_REGISTER(Image)
    METHOD_REGISTER(Image, clone)
    METHOD_REGISTER(Image, getBounds)
    METHOD_REGISTER(Image, getWidth)
    METHOD_REGISTER(Image, getHeight)
    METHOD_REGISTER(Image, getPalette)
    METHOD_REGISTER(Image, setPalette)
    METHOD_REGISTER(Image, getPaletteSize)
    METHOD_REGISTER(Image, getPixelFormat)
    METHOD_REGISTER(Image, getRawFormat)
    METHOD_REGISTER(Image, rotateFlip)
    METHOD_REGISTER(Image, save)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(Image)
    CONSTRUCTOR_REGISTER(Image)
    METHOD_REGISTER(Image, loadFile)
METHOD_REGEND

MIL_Image* LoadImageFromFile(const char* file)
{
    if (NULL != file) {
        Image* img = (Image*)New(Image);
        if (NULL != img) {
            if (MIL_OK == _MC(img)->loadFile(img, file)) {
                return (MIL_Image*)img;
            }
            else {
                Delete(img);
            }
        }
    }

    return NULL;
}

