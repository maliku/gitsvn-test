/*! ============================================================================
 * @file bitmap.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "image.h"

CONSTRUCTOR(MIL_DIBitmap)
{
    _private(MIL_DIBitmap)->data = NULL;
    return self;
}

DESTRUCTOR(MIL_DIBitmap)
{

}

MIL_Status  MIL_DIBitmap_X_getPixel(_Self(MIL_DIBitmap), int x, int y, MIL_Color* color)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_DIBitmap_X_lockBits(_Self(MIL_DIBitmap), const MIL_Rect* rc, MIL_DIBitmapData* locked_data)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_DIBitmap_X_setPixel(_Self(MIL_DIBitmap), int x, int y, const MIL_Color* color)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Status  MIL_DIBitmap_X_unlockBits(_Self(MIL_DIBitmap), MIL_DIBitmapData* locked_data)
{
    return MIL_NOT_IMPLEMENTED;
}

BEGIN_METHOD_MAP(MIL_DIBitmap, MIL_Image)
    CONSTRUCTOR_MAP(MIL_DIBitmap)
    DESTRUCTOR_MAP(MIL_DIBitmap)

    METHOD_PLACEHOLDER(addHoldRef)
    METHOD_PLACEHOLDER(decHoldRef)
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

    /* Add new method */
    METHOD_MAP(MIL_DIBitmap, getPixel)
    METHOD_MAP(MIL_DIBitmap, lockBits)
    METHOD_MAP(MIL_DIBitmap, setPixel)
    METHOD_MAP(MIL_DIBitmap, unlockBits)
END_METHOD_MAP




MIL_DIBitmap* LoadBitmapFromFile(const char* file)
{
    if (NULL != file) {
        MIL_DIBitmap* bmp = (MIL_DIBitmap*)New(MIL_DIBitmap);
        if (NULL != bmp) {
            if (MIL_OK == _vc1((MIL_Image*)bmp, loadFile, file)) {
                return (MIL_DIBitmap*)bmp;
            }
            else {
                Delete(bmp);
            }
        }
    }

    return NULL;
}

