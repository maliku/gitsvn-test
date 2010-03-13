/*!============================================================================
 * @file MIL_mutex.h 
 * @Synopsis Mutex base class. 
 * @author DongKai
 * @version 1.0
 * @date 2010年03月13日 16时44分24秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  MIL_MUTEX_INC
#define  MIL_MUTEX_INC
#include "MIL_stdinc.h"
#include "coo.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/** Synchronization functions which can time out return this value
 *  if they time out.
 */
#define SDL_MUTEX_TIMEDOUT	1

/** This is the timeout value which corresponds to never time out */
#define SDL_MUTEX_MAXWAIT	(~(Uint32)0)

CLASS(Mutex)
{
    VIRTUAL_FUNCTION_DECLARE_BEGIN(Mutex)
        VIRTUAL_FUNCTION_DECLARE_END

        MEMBER_FUNCTION_DECLARE_BEGIN(Mutex)
        MEMBER_FUNCTION_DECLARE_END
};

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef MIL_MUTEX_INC  ----- */
