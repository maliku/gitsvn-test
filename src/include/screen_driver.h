/*!============================================================================
 * @file screen_driver.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  SCREEN_DRIVER_INC
#define  SCREEN_DRIVER_INC

#include "MIL_paint.h"
#include "screen.h"

CLASS(MScreenDriverFactory)
{
    BEGIN_METHOD_DECLARE(MScreenDriverFactory)
    MScreen* (*create)(_Self(MScreenDriverFactory), const char* name, int);
    char*  (*names)(_Self(MScreenDriverFactory));
    END_METHOD_DECLARE
};

#endif   /* ----- #ifndef SCREEN_DRIVER_INC  ----- */

