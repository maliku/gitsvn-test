/*! ============================================================================
 * @file Win32Mutex.c 
 * @Synopsis Mutex implementation for pthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13
 *  Organization: http://www.ds0101.net
 */

#include "Win32Mutex.h"

#if MIL_THREAD_WIN32	
CONSTRUCTOR(Win32Mutex)
{
    /* Create the mutex, with initial value signaled */
    if (self) {
        self->id = CreateMutex(NULL, FALSE, NULL);
        if ( ! self->id ) {
            MIL_SetError("Couldn't create mutex");
            MIL_free(self);
            self = NULL;
        }
    }
    return(self);
}

DESTRUCTOR(Win32Mutex)
{
    if (self) {
        if (self->id) {
            CloseHandle(self->id);
            self->id = 0;
        }
    }
}

Sint32 METHOD_NAMED(Win32Mutex, lock)(_SELF)
{
	if ( WaitForSingleObject(self->id, INFINITE) == WAIT_FAILED ) {
		MIL_SetError("Couldn't wait on mutex");
		return -1;
	}
	return(0);
}

Sint32 METHOD_NAMED(Win32Mutex, unlock)(_SELF)
{
	if ( self == NULL ) {
		MIL_SetError("Passed a NULL mutex");
		return -1;
	}
	if ( ReleaseMutex(self->id) == FALSE ) {
		MIL_SetError("Couldn't release mutex");
		return -1;
	}
	return(0);
}

VIRTUAL_METHOD_REGBEGIN(Win32Mutex, MIL_mutex)
    DESTRUCTOR_REGISTER(Win32Mutex)
    METHOD_REGISTER(Win32Mutex, lock)
    METHOD_REGISTER(Win32Mutex, unlock)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(Win32Mutex)
    CONSTRUCTOR_REGISTER(Win32Mutex)
METHOD_REGEND
#endif

