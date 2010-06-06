/*!============================================================================
 * @file pixels_ops.h 
 * @Synopsis Pixels operations. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  PIXELS_OPS_INC
#define  PIXELS_OPS_INC

#include "MIL_gdi.h"

BEGIN_CLASS_INHERIT(PixelsSet1, MIL_PixelsOps)
    NO_METHOD_EXPAND(PixelsSet1)
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(PixelsSet2, MIL_PixelsOps)
    NO_METHOD_EXPAND(PixelsSet2)
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(PixelsSet3, MIL_PixelsOps)
    NO_METHOD_EXPAND(PixelsSet3)
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(PixelsSet4, MIL_PixelsOps)
    NO_METHOD_EXPAND(PixelsSet4)
END_CLASS_INHERIT

MIL_PixelsOps* GetPixelsOperator(MIL_PixelsOperation opt, int bpp);
#endif   /* ----- #ifndef PIXELS_OPS_INC  ----- */

