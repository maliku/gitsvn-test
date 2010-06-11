/*! ============================================================================
 * @file DummyMutex.c 
 * @Synopsis Mutex implementation for pthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13
 *  Organization: http://www.ds0101.net
 */

#include "dummy_mutex.h"

Sint32 DummyMutex_X_lock(_SELF)
{
	return 0;
}

Sint32 DummyMutex_X_unlock(_SELF)
{
	return 0;

}

BEGIN_METHOD_MAP(DummyMutex, MIL_mutex)
    METHOD_MAP(DummyMutex, lock)
    METHOD_MAP(DummyMutex, unlock)
END_METHOD_MAP




MIL_mutex* CreateDummyMutex()
{
    return (MIL_mutex*)New(DummyMutex);
}
