/*!============================================================================
 * @file bitmap.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  BITMAP_INC
#define  BITMAP_INC

#include "image.h"

CLASS_INHERIT_BEGIN(Bitmap, MIL_Bitmap)
    METHOD_DECLARE_PLACEHOLDER(Bitmap)

    PRIVATE_BEGIN(Bitmap)
        MIL_BitmapData* data;
    PRIVATE_END
CLASS_INHERIT_END
#endif   /* ----- #ifndef BITMAP_INC  ----- */

