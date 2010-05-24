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

METHOD_MAP_BEGIN(DummyMutex, MIL_mutex)
    NON_CONSTRUCTOR
    NON_DESTRUCTOR
    METHOD_MAP(DummyMutex, lock)
    METHOD_MAP(DummyMutex, unlock)
METHOD_MAP_END




MIL_mutex* CreateDummyMutex()
{
    return (MIL_mutex*)New(DummyMutex);
}
