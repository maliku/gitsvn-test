/*!============================================================================
 * @file mutex.h 
 * @Synopsis mutex common head file.
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  MUTEX_INC
#define  MUTEX_INC

#include "MIL_mutex.h"

MIL_mutex* CreateDummyMutex();
MIL_mutex* CreateWin32Mutex();
MIL_mutex* CreatePthreadMutex();

#endif   /* ----- #ifndef MUTEX_INC  ----- */

