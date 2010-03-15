/*!============================================================================
 * @file DummyMutex.h 
 * @Synopsis mutex implementation with libpthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010年03月13日 19时41分50秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  DUMMYMUTEX_INC
#define  DUMMYMUTEX_INC

#include "MIL_config.h"
#include "MIL_mutex.h"

CLASS_INHERIT_BEGIN(DummyMutex, MIL_mutex)
    METHOD_DECLARE_PLACEHOLDER(DummyMutex)
CLASS_INHERIT_END


#endif   /* ----- #ifndef DUMMYMUTEX_INC  ----- */
