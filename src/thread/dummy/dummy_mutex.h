/*!============================================================================
 * @file DummyMutex.h 
 * @Synopsis mutex implementation with libpthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13 19:41:50 
 *  Organization: http://www.ds0101.net
 */

#ifndef  DUMMYMUTEX_INC
#define  DUMMYMUTEX_INC

#include "MIL_config.h"
#include "MIL_mutex.h"

BEGIN_CLASS_INHERIT(DummyMutex, MIL_mutex)
    NO_METHOD_EXPAND(DummyMutex)
    
END_CLASS_INHERIT


#endif   /* ----- #ifndef DUMMYMUTEX_INC  ----- */
