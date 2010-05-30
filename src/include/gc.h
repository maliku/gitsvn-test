/*!============================================================================
 * @file gfx_context.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  GFX_CONTEXT_INC
#define  GFX_CONTEXT_INC

#include "gdi.h"
#include "surface.h"

BEGIN_CLASS_INHERIT(MemoryGC, MIL_GraphicsContext)
    BEGIN_METHOD_EXPAND_DECLARE(MemoryGC)
#define MIL_MEMGC_METHOD_TABLE \
        MIL_GC_METHOD_TABLE
        MIL_MEMGC_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE

    BEGIN_PRIVATE(MemoryGC)
    MIL_Image* image;
    MIL_Pen* pen;
    MIL_Brush* brush;
    END_PRIVATE
END_CLASS_INHERIT

#endif   /* ----- #ifndef GFX_CONTEXT_INC  ----- */

