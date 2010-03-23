/*!============================================================================
 * @file gfx.h 
 * @Synopsis abstract graphics layer.
 * @author DongKai
 * @version 1.0
 * @date 2010年03月23日 22时50分46秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  GFX_INC
#define  GFX_INC

#include "coo.h"
#include "MIL_surface.h"

CLASS(VideoDevice) {
    VIRTUAL_METHOD_DECLARE_BEGIN(VideoDevice) 
        int (*VideoInitialize)(_Self(VideoDevice));
        MIL_Rect** (*VideoModeList)(_Self(VideoDevice));
        MIL_Surface* (*SetVideoMode)(_Self(VideoDevice));
        int (*ToggleFullScreen)(_Self(VideoDevice));
        void (*VideoQuit)(_Self(VideoDevice));
        void (*UpdateRects)(_Self(VideoDevice), int numrects, MIL_Rect *rects);

    VIRTUAL_METHOD_DECLARE_END 

        METHOD_DECLARE_PLACEHOLDER(VideoDevice)

        const char *name;
};

#endif   /* ----- #ifndef GFX_INC  ----- */

