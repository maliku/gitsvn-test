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
    _private(Image)->surface = NULL;
    return self;
}

DESTRUCTOR(Image)
{

}

MIL_Image*  Image_X_clone(_Self(MIL_Image))
{
    return NULL;
}

MIL_Status  Image_X_getBounds(_Self(MIL_Image), MIL_Rect* rc)
{
    return MIL_NOT_IMPLEMENTED;
}

Uint32      Image_X_getWidth(_Self(MIL_Image))
{
    return 0;
}

Uint32      Image_X_getHeight(_Self(MIL_Image))
{
    return 0;
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
    return MIL_NOT_IMPLEMENTED;
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
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status Image_X_loadFile(_SELF, const char* file)
{
    if (NULL != file) {
        MIL_RWops* ops;
        if (NULL != _private(Image)->surface) {
            Delete(_private(Image));
            _private(Image)->surface = NULL;
        }
        ops = MIL_RWFromFile(file, "rb");
        if (NULL != ops) {
            _private(Image)->surface = MIL_LoadBMP_RW(ops, 1);
            Delete(ops);
            return NULL != _private(Image)->surface ? MIL_OK : MIL_OUT_OF_MEMORY;
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

