/*!============================================================================
 * @file img_loader.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  IMG_LOADER_INC
#define  IMG_LOADER_INC

#include "coo.h"
#include "MIL_rwops.h"
#include "MIL_gdi.h"

INTERFACE_BEGIN(ImageLoader)
#define IMAGELOADER_METHOD_TABLE \
    MIL_Image* (*load)(_Self(ImageLoader), MIL_RWops*); \
    MIL_Status (*save)(_Self(ImageLoader), MIL_Image*, MIL_RWops*);
INTERFACE_END

CLASS_INHERIT_BEGIN(BmpLoader, ImageLoader)
    NO_METHOD_EXPAND(BmpLoader)
CLASS_INHERIT_END

#endif   /* ----- #ifndef IMG_LOADER_INC  ----- */

