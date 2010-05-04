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

VIRTUAL_METHOD_REGBEGIN(DummyMutex, MIL_mutex)
    NON_DESTRUCTOR
    METHOD_REGISTER(DummyMutex, lock)
    METHOD_REGISTER(DummyMutex, unlock)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(DummyMutex)
    NON_CONSTRUCTOR
METHOD_REGEND

MIL_mutex* CreateDummyMutex()
{
    return (MIL_mutex*)New(DummyMutex);
}