/*!============================================================================
 * @file MIL_mutex.h 
 * @Synopsis MIL_mutex base class. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13 16:44:24 
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

/* #if !MIL_THREAD_WIN32 && !HAVE_PTHREAD
 * #define MIL_THREAD_SUPPORTED
 * #endif
 */

CLASS(MIL_mutex)
{
    BEGIN_METHOD_DECLARE(MIL_mutex)
        Sint32 (*lock)(_SELF);
        Sint32 (*unlock)(_SELF);
        END_METHOD_DECLARE
};

MIL_mutex * MIL_CreateMutex();
void MIL_DestroyMutex(MIL_mutex*);

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef MIL_MUTEX_INC  ----- */
