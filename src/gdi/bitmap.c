/*! ============================================================================
 * @file bitmap.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "bitmap.h"

CONSTRUCTOR(Bitmap)
{
    _private(Bitmap)->data = NULL;
    return self;
}

DESTRUCTOR(Bitmap)
{

}

MIL_Status  MIL_Bitmap_X_getPixel(_Self(MIL_Bitmap), int x, int y, MIL_Color* color)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_Bitmap_X_lockBits(_Self(MIL_Bitmap), const MIL_Rect* rc, MIL_BitmapData* locked_data)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_Bitmap_X_setPixel(_Self(MIL_Bitmap), int x, int y, const MIL_Color* color)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_Bitmap_X_unlockBits(_Self(MIL_Bitmap), MIL_BitmapData* locked_data)
{
    return MIL_NOT_IMPLEMENTED;
}

VIRTUAL_METHOD_REGISTER_PLACEHOLDER(MIL_Bitmap, Image)
METHOD_REGBEGIN(MIL_Bitmap)
    NON_CONSTRUCTOR
    METHOD_REGISTER(MIL_Bitmap, getPixel)
    METHOD_REGISTER(MIL_Bitmap, lockBits)
    METHOD_REGISTER(MIL_Bitmap, setPixel)
    METHOD_REGISTER(MIL_Bitmap, unlockBits)
METHOD_REGEND


VIRTUAL_METHOD_REGBEGIN(Bitmap, MIL_Bitmap)
    DESTRUCTOR_REGISTER(Bitmap)
    METHOD_PLACEHOLDER(clone)
    METHOD_PLACEHOLDER(getBounds)
    METHOD_PLACEHOLDER(getWidth)
    METHOD_PLACEHOLDER(getHeight)
    METHOD_PLACEHOLDER(getPalette)
    METHOD_PLACEHOLDER(setPalette)
    METHOD_PLACEHOLDER(getPaletteSize)
    METHOD_PLACEHOLDER(getPixelFormat)
    METHOD_PLACEHOLDER(getRawFormat)
    METHOD_PLACEHOLDER(rotateFlip)
    METHOD_PLACEHOLDER(save)
VIRTUAL_METHOD_REGEND

METHOD_REGISTER_BEGIN(Bitmap)
    CONSTRUCTOR_REGISTER(Bitmap)
METHOD_REGISTER_END

MIL_Bitmap* LoadBitmapFromFile(const char* file)
{
    if (NULL != file) {
        Bitmap* bmp = (Bitmap*)New(Image);
        if (NULL != bmp) {
            if (MIL_OK == _MC((Image*)bmp)->loadFile(bmp, file)) {
                return (MIL_Bitmap*)bmp;
            }
            else {
                Delete(bmp);
            }
        }
    }

    return NULL;
}

