/*!============================================================================
 * @file PthreadMutex.h 
 * @Synopsis mutex implementation with libpthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010年03月13日 19时41分50秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  PTHREADMUTEX_INC
#define  PTHREADMUTEX_INC

#include "MIL_mutex.h"
    CLASS_INHERIT_BEGIN(PthreadMutex, MIL_mutex)
METHOD_DECLARE_PLACEHOLDER(PthreadMutex)

    pthread_mutex_t id;
#if FAKE_RECURSIVE_MUTEX
    int recursive;
    pthread_t owner;
#endif

    CLASS_INHERIT_END

#endif   /* ----- #ifndef PTHREADMUTEX_INC  ----- */
