/*!============================================================================
 * @file Win32Mutex.h 
 * @Synopsis mutex implementation with libpthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010年03月13日 19时41分50秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  WIN32MUTEX_INC
#define  WIN32MUTEX_INC
#include "MIL_config.h"

/* Mutex functions using the Win32 API */

#if MIL_THREAD_WIN32	

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "MIL_mutex.h"

CLASS_INHERIT_BEGIN(Win32Mutex, MIL_mutex)
    METHOD_DECLARE_PLACEHOLDER(Win32Mutex)

	HANDLE id;
CLASS_INHERIT_END

#endif

#endif   /* ----- #ifndef WIN32MUTEX_INC  ----- */
