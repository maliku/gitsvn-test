/*! ============================================================================
 * @file MIL_mutex.c 
 * @Synopsis Mutex factory. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13
 *  Organization: http://www.ds0101.net
 */

#include "mutex.h"

MIL_mutex* MIL_CreateMutex()
{
#if MIL_THREAD_WIN32
    puts("Create win32 mutex");
    return CreateWin32Mutex();
#elif HAVE_PTHREAD
    puts("Create pthread mutex");
    return CreatePthreadMutex();
#else
    puts("Create dummy mutex");
    return CreateDummyMutex();
#endif
}

void MIL_DestroyMutex(MIL_mutex * mutex)
{
    Delete(mutex);
}

MAKE_PURE_VIRTUAL_CLASS(MIL_mutex)
