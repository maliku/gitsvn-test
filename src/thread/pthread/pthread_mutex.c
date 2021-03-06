/*! ============================================================================
 * @file PthreadMutex.c 
 * @Synopsis Mutex implementation for pthread. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-13
 *  Organization: http://www.ds0101.net
 */

#include "pthread_mutex.h"

#if HAVE_PTHREAD

CONSTRUCTOR(PthreadMutex)
{
	pthread_mutexattr_t attr;

	/* Allocate the structure */
	if ( self ) {
		pthread_mutexattr_init(&attr);
#if MIL_THREAD_PTHREAD_RECURSIVE_MUTEX
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#elif MIL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP
		pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#else
		/* No extra attributes necessary */
#endif
		if ( pthread_mutex_init(&self->id, &attr) != 0 ) {
			MIL_SetError("pthread_mutex_init() failed");

			MIL_free(self);
			self = NULL;
		}
	} else {
/* 		MIL_OutOfMemory();
 */
	}
}

DESTRUCTOR(PthreadMutex)
{
    PthreadMutex * mutex = (PthreadMutex*)self;
	if (mutex) {
		pthread_mutex_destroy(&mutex->id);
	}
}

Sint32 PthreadMutex_X_lock(_SELF)
{
	int retval;
    PthreadMutex* mutex = (PthreadMutex*)self;
#if FAKE_RECURSIVE_MUTEX
	pthread_t this_thread;
#endif

	if ( mutex == NULL ) {
		MIL_SetError("Passed a NULL mutex");
		return -1;
	}

	retval = 0;
#if FAKE_RECURSIVE_MUTEX
	this_thread = pthread_self();
	if ( mutex->owner == this_thread ) {
		++mutex->recursive;
	} else {
		/* The order of operations is important.
		   We set the locking thread id after we obtain the lock
		   so unlocks from other threads will fail.
		*/
		if ( pthread_mutex_lock(&mutex->id) == 0 ) {
			mutex->owner = this_thread;
			mutex->recursive = 0;
		} else {
			MIL_SetError("pthread_mutex_lock() failed");
			retval = -1;
		}
	}
#else
	if ( pthread_mutex_lock(&mutex->id) < 0 ) {
		MIL_SetError("pthread_mutex_lock() failed");
		retval = -1;
	}
#endif
	return retval;
}

Sint32 PthreadMutex_X_unlock(_SELF)
{
	int retval;
    PthreadMutex* mutex = (PthreadMutex*)self;

	if ( mutex == NULL ) {
		MIL_SetError("Passed a NULL mutex");
		return -1;
	}

	retval = 0;
#if FAKE_RECURSIVE_MUTEX
	/* We can only unlock the mutex if we own it */
	if ( pthread_self() == mutex->owner ) {
		if ( mutex->recursive ) {
			--mutex->recursive;
		} else {
			/* The order of operations is important.
			   First reset the owner so another thread doesn't lock
			   the mutex and set the ownership before we reset it,
			   then release the lock semaphore.
			 */
			mutex->owner = 0;
			pthread_mutex_unlock(&mutex->id);
		}
	} else {
		MIL_SetError("mutex not owned by this thread");
		retval = -1;
	}

#else
	if ( pthread_mutex_unlock(&mutex->id) < 0 ) {
		MIL_SetError("pthread_mutex_unlock() failed");
		retval = -1;
	}
#endif /* FAKE_RECURSIVE_MUTEX */

	return retval;

}

BEGIN_METHOD_MAP(PthreadMutex, MIL_mutex)
    CONSTRUCTOR_MAP(PthreadMutex)
    DESTRUCTOR_MAP(PthreadMutex)
    METHOD_MAP(PthreadMutex, lock)
    METHOD_MAP(PthreadMutex, unlock)
END_METHOD_MAP




MIL_mutex* CreatePthreadMutex()
{
    return (MIL_mutex*)New(PthreadMutex);
}

#endif
