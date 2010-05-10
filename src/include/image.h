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


CLASS_INHERIT_BEGIN(Image, MIL_Image)
    METHOD_DECLARE_BEGIN(Image)
        MIL_Status (*loadFile)(_SELF, const char* file);
    METHOD_DECLARE_END

    PRIVATE_BEGIN(Image)
        Surface* surface;
    PRIVATE_END
CLASS_INHERIT_END
#endif   /* ----- #ifndef IMAGE_INC  ----- */

