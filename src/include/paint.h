/*!============================================================================
 * @file paint.h 
 * @Synopsis Paint object implementation. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  PAINT_INC
#define  PAINT_INC

#include "MIL_gdi.h"

BEGIN_CLASS_INHERIT(SolidPen, MIL_Pen)
    NO_METHOD_EXPAND(SolidPen)

    MIL_Color* color;
    MIL_DashStyle dash;
    MIL_LineJoin line_join;
    Uint32 width;
    MIL_Brush* brush;
END_CLASS_INHERIT

#endif   /* ----- #ifndef PAINT_INC  ----- */

