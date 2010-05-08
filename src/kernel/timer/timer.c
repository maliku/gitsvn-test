/*
    MIL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "MIL_config.h"

#include "timer_c.h"
#include "MIL_mutex.h"
#include "systimer.h"

/* #define DEBUG_TIMERS */

int MIL_timer_started = 0;
int MIL_timer_running = 0;

/* Data to handle a single periodic alarm */
Uint32 MIL_alarm_interval = 0;
MIL_TimerCallback MIL_alarm_callback;

/* Data used for a thread-based timer */
static int MIL_timer_threaded = 0;

struct _MIL_TimerID {
	Uint32 interval;
	MIL_NewTimerCallback cb;
	void *param;
	Uint32 last_alarm;
	struct _MIL_TimerID *next;
};

static MIL_TimerID MIL_timers = NULL;
static MIL_mutex *MIL_timer_mutex;
static volatile MIL_bool list_changed = MIL_FALSE;

/* Set whether or not the timer should use a thread.
   This should not be called while the timer subsystem is running.
*/
int MIL_SetTimerThreaded(int value)
{
	int retval;

	if ( MIL_timer_started ) {
		MIL_SetError("Timer already initialized");
		retval = -1;
	} else {
		retval = 0;
		MIL_timer_threaded = value;
	}
	return retval;
}

int MIL_TimerInit(void)
{
	int retval;

	retval = 0;
	if ( MIL_timer_started ) {
		MIL_TimerQuit();
	}
	if ( ! MIL_timer_threaded ) {
		retval = MIL_SYS_TimerInit();
	}
	if ( MIL_timer_threaded ) {
		MIL_timer_mutex = MIL_CreateMutex();
	}
	if ( retval == 0 ) {
		MIL_timer_started = 1;
	}
	return(retval);
}

void MIL_TimerQuit(void)
{
	MIL_SetTimer(0, NULL);
	if ( MIL_timer_threaded < 2 ) {
		MIL_SYS_TimerQuit();
	}
	if ( MIL_timer_threaded ) {
		MIL_DestroyMutex(MIL_timer_mutex);
		MIL_timer_mutex = NULL;
	}
	MIL_timer_started = 0;
	MIL_timer_threaded = 0;
}

void MIL_ThreadedTimerCheck(void)
{
	Uint32 now, ms;
	MIL_TimerID t, prev, next;
	MIL_bool removed;

	MIL_mutexP(MIL_timer_mutex);
	list_changed = MIL_FALSE;
	now = MIL_GetTicks();
	for ( prev = NULL, t = MIL_timers; t; t = next ) {
		removed = MIL_FALSE;
		ms = t->interval - MIL_TIMESLICE;
		next = t->next;
		if ( (int)(now - t->last_alarm) > (int)ms ) {
			struct _MIL_TimerID timer;

			if ( (now - t->last_alarm) < t->interval ) {
				t->last_alarm += t->interval;
			} else {
				t->last_alarm = now;
			}
#ifdef DEBUG_TIMERS
			printf("Executing timer %p (thread = %d)\n",
				t, MIL_ThreadID());
#endif
			timer = *t;
			MIL_mutexV(MIL_timer_mutex);
			ms = timer.cb(timer.interval, timer.param);
			MIL_mutexP(MIL_timer_mutex);
			if ( list_changed ) {
				/* Abort, list of timers modified */
				/* FIXME: what if ms was changed? */
				break;
			}
			if ( ms != t->interval ) {
				if ( ms ) {
					t->interval = ROUND_RESOLUTION(ms);
				} else {
					/* Remove timer from the list */
#ifdef DEBUG_TIMERS
					printf("MIL: Removing timer %p\n", t);
#endif
					if ( prev ) {
						prev->next = next;
					} else {
						MIL_timers = next;
					}
					MIL_free(t);
					--MIL_timer_running;
					removed = MIL_TRUE;
				}
			}
		}
		/* Don't update prev if the timer has disappeared */
		if ( ! removed ) {
			prev = t;
		}
	}
	MIL_mutexV(MIL_timer_mutex);
}

static MIL_TimerID MIL_AddTimerInternal(Uint32 interval, MIL_NewTimerCallback callback, void *param)
{
	MIL_TimerID t;
	t = (MIL_TimerID) MIL_malloc(sizeof(struct _MIL_TimerID));
	if ( t ) {
		t->interval = ROUND_RESOLUTION(interval);
		t->cb = callback;
		t->param = param;
		t->last_alarm = MIL_GetTicks();
		t->next = MIL_timers;
		MIL_timers = t;
		++MIL_timer_running;
		list_changed = MIL_TRUE;
	}
#ifdef DEBUG_TIMERS
	printf("MIL_AddTimer(%d) = %08x num_timers = %d\n", interval, (Uint32)t, MIL_timer_running);
#endif
	return t;
}

MIL_TimerID MIL_AddTimer(Uint32 interval, MIL_NewTimerCallback callback, void *param)
{
	MIL_TimerID t;
	if ( ! MIL_timer_mutex ) {
		if ( MIL_timer_started ) {
			MIL_SetError("This platform doesn't support multiple timers");
		} else {
			MIL_SetError("You must call MIL_Init(MIL_INIT_TIMER) first");
		}
		return NULL;
	}
	if ( ! MIL_timer_threaded ) {
		MIL_SetError("Multiple timers require threaded events!");
		return NULL;
	}
	MIL_mutexP(MIL_timer_mutex);
	t = MIL_AddTimerInternal(interval, callback, param);
	MIL_mutexV(MIL_timer_mutex);
	return t;
}

MIL_bool MIL_RemoveTimer(MIL_TimerID id)
{
	MIL_TimerID t, prev = NULL;
	MIL_bool removed;

	removed = MIL_FALSE;
	MIL_mutexP(MIL_timer_mutex);
	/* Look for id in the linked list of timers */
	for (t = MIL_timers; t; prev=t, t = t->next ) {
		if ( t == id ) {
			if(prev) {
				prev->next = t->next;
			} else {
				MIL_timers = t->next;
			}
			MIL_free(t);
			--MIL_timer_running;
			removed = MIL_TRUE;
			list_changed = MIL_TRUE;
			break;
		}
	}
#ifdef DEBUG_TIMERS
	printf("MIL_RemoveTimer(%08x) = %d num_timers = %d thread = %d\n", (Uint32)id, removed, MIL_timer_running, MIL_ThreadID());
#endif
	MIL_mutexV(MIL_timer_mutex);
	return removed;
}

/* Old style callback functions are wrapped through this */
static Uint32 MILCALL callback_wrapper(Uint32 ms, void *param)
{
	MIL_TimerCallback func = (MIL_TimerCallback) param;
	return (*func)(ms);
}

int MIL_SetTimer(Uint32 ms, MIL_TimerCallback callback)
{
	int retval;

#ifdef DEBUG_TIMERS
	printf("MIL_SetTimer(%d)\n", ms);
#endif
	retval = 0;

	if ( MIL_timer_threaded ) {
		MIL_mutexP(MIL_timer_mutex);
	}
	if ( MIL_timer_running ) {	/* Stop any currently running timer */
		if ( MIL_timer_threaded ) {
			while ( MIL_timers ) {
				MIL_TimerID freeme = MIL_timers;
				MIL_timers = MIL_timers->next;
				MIL_free(freeme);
			}
			MIL_timer_running = 0;
			list_changed = MIL_TRUE;
		} else {
			MIL_SYS_StopTimer();
			MIL_timer_running = 0;
		}
	}
	if ( ms ) {
		if ( MIL_timer_threaded ) {
			if ( MIL_AddTimerInternal(ms, callback_wrapper, (void *)callback) == NULL ) {
				retval = -1;
			}
		} else {
			MIL_timer_running = 1;
			MIL_alarm_interval = ms;
			MIL_alarm_callback = callback;
			retval = MIL_SYS_StartTimer();
		}
	}
	if ( MIL_timer_threaded ) {
		MIL_mutexV(MIL_timer_mutex);
	}

	return retval;
}
