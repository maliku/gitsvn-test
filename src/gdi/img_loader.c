/*! ============================================================================
 * @file img_loader.c 
 * @Synopsis Type of image file format loader. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "img_loader.h"

MAKE_PURE_VIRTUAL_CLASS(ImageLoader)

MIL_Image* BmpLoader_X_load(_Self(ImageLoader), MIL_RWops* src)
{

}

MIL_Status BmpLoader_X_save(_Self(ImageLoader), MIL_Image*src, MIL_RWops* dst)
{

}
