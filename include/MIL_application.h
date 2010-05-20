/*!============================================================================
 * @file MIL_application.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-09- 15:22:29 
 *  Organization: http://www.ds0101.net
 */

#ifndef  MIL_APPLICATION_INC
#define  MIL_APPLICATION_INC
#include "coo.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

CLASS(MIL_Application)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(MIL_Application)
    VIRTUAL_METHOD_DECLARE_END

    
};

MIL_Application* CreateApplication(char **args, int num);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef MIL_APPLICATION_INC  ----- */

