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

BEGIN_INTERFACE(ImageLoader)
#define IMAGELOADER_METHOD_TABLE \
    MIL_Image* (*load)(_Self(ImageLoader), MIL_RWops*); \
    MIL_Status (*save)(_Self(ImageLoader), MIL_Image*, MIL_RWops*);
END_INTERFACE

BEGIN_CLASS_INHERIT(BmpLoader, ImageLoader)
    NO_METHOD_EXPAND(BmpLoader)
END_CLASS_INHERIT

#endif   /* ----- #ifndef IMG_LOADER_INC  ----- */

