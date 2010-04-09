/*!============================================================================
 * @file MIL_application.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日 15时22分29秒 
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef  MIL_APPLICATION_INC
#define  MIL_APPLICATION_INC
#include "coo.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
struct VideoDevice;
CLASS(MIL_Application)
{
    VIRTUAL_METHOD_DECLARE_PLACEHOLDER(MIL_Application)

    METHOD_DECLARE_BEGIN(MIL_Application)
    METHOD_DECLARE_END

    struct VideoDevice* video_device;
};

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef MIL_APPLICATION_INC  ----- */

