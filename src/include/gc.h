/*!============================================================================
 * @file gfx_context.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  GFX_CONTEXT_INC
#define  GFX_CONTEXT_INC

#include "MIL_gdi.h"
#include "surface.h"

CLASS_INHERIT_BEGIN(MemoryGC, MIL_GraphicsContext)
    METHOD_EXPAND_DECLARE_BEGIN(MemoryGC)
#define MIL_MEMGC_METHOD_TABLE \
        MIL_GC_METHOD_TABLE
        MIL_MEMGC_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END

    PRIVATE_BEGIN(MemoryGC)
    Surface* surface;
    MIL_Pen* pen;
    MIL_Brush* brush;
    PRIVATE_END
CLASS_INHERIT_END

#endif   /* ----- #ifndef GFX_CONTEXT_INC  ----- */

