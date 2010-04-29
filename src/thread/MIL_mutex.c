/*! ============================================================================
 * @file MIL_mutex.c 
 * @Synopsis Mutex factory. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13
 *  Organization: http://www.ds0101.net
 */

#include "pthread/PthreadMutex.h"
#include "win32/Win32Mutex.h"

MIL_mutex* MIL_CreateMutex()
{
#if MIL_THREAD_WIN32
    return New(Win32Mutex);
#elif HAVE_PTHREAD
    return New(PthreadMutex);
#else
    return New(DummyMutex);
#endif
}

void MIL_DestroyMutex(MIL_mutex * mutex)
{
    Delete(mutex);
}

MAKE_PURE_VIRTUAL_CLASS(MIL_mutex)
METHOD_REGISTER_PLACEHOLDER(MIL_mutex)
