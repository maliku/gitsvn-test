/*! ============================================================================
 * @file DummyMutex.c 
 * @Synopsis Mutex implementation for pthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010年03月13日
 *  Organization: http://www.ds0101.net
 */

#include "DummyMutex.h"

Sint32 METHOD_NAMED(DummyMutex, lock)(_Self(MIL_mutex))
{
	return 0;
}

Sint32 METHOD_NAMED(DummyMutex, unlock)(_Self(MIL_mutex))
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

