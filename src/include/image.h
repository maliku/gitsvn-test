/*!============================================================================
 * @file image.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  IMAGE_INC
#define  IMAGE_INC

#include "MIL_gdi.h"
#include "surface.h"

#if 0
BEGIN_CLASS_INHERIT(Image, MIL_Image)
    NO_METHOD_EXPAND(Image)
END_CLASS_INHERIT
#endif

MIL_Image*
CreateImageFromSurface(Surface* surface);
#endif   /* ----- #ifndef IMAGE_INC  ----- */

