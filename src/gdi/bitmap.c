/*! ============================================================================
 * @file bitmap.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "image.h"

CONSTRUCTOR(MIL_Bitmap)
{
    _private(MIL_Bitmap)->data = NULL;
    return self;
}

DESTRUCTOR(MIL_Bitmap)
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

VIRTUAL_METHOD_REGBEGIN(MIL_Bitmap, MIL_Image)
    DESTRUCTOR_REGISTER(MIL_Bitmap)
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

METHOD_REGBEGIN(MIL_Bitmap)
    CONSTRUCTOR_REGISTER(MIL_Bitmap)
    METHOD_REGISTER(MIL_Bitmap, getPixel)
    METHOD_REGISTER(MIL_Bitmap, lockBits)
    METHOD_REGISTER(MIL_Bitmap, setPixel)
    METHOD_REGISTER(MIL_Bitmap, unlockBits)
METHOD_REGEND

MIL_Bitmap* LoadBitmapFromFile(const char* file)
{
    if (NULL != file) {
        MIL_Bitmap* bmp = (MIL_Bitmap*)New(MIL_Bitmap);
        if (NULL != bmp) {
            if (MIL_OK == _vc1((MIL_Image*)bmp, loadFile, file)) {
                return (MIL_Bitmap*)bmp;
            }
            else {
                Delete(bmp);
            }
        }
    }

    return NULL;
}

