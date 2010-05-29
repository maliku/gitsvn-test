/*!============================================================================
 * @file PthreadMutex.h 
 * @Synopsis mutex implementation with libpthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13 19:41:50 
 *  Organization: http://www.ds0101.net
 */

#ifndef  PTHREADMUTEX_INC
#define  PTHREADMUTEX_INC

#include "MIL_config.h"
#if HAVE_PTHREAD
#include <pthread.h>
#include "MIL_mutex.h"

BEGIN_CLASS_INHERIT(PthreadMutex, MIL_mutex)
    NO_METHOD_EXPAND(PthreadMutex)
    

    pthread_mutex_t id;
#if FAKE_RECURSIVE_MUTEX
    int recursive;
    pthread_t owner;
#endif
END_CLASS_INHERIT

#endif

#endif   /* ----- #ifndef PTHREADMUTEX_INC  ----- */
