/*
    MIL - A embedded system graphics Middle Interface Layer.
    Copyright (C) 2009-2010 Dong Kai

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

    Dong Kai
    libmilui@gmail.com
*/
#include "MIL_config.h"

#ifdef MIL_TIMER_RISCOS

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "MIL_timer.h"
#include "../MIL_timer_c.h"

#if MIL_THREADS_DISABLED
/* Timer  MIL_arraysize(Timer ),start/reset time */
static Uint32 timerStart;
/* Timer running function */
void RISCOS_CheckTimer();
#else
#include <pthread.h>
extern Uint32 riscos_main_thread;
extern int riscos_using_threads;
extern Uint32 MIL_ThreadID();
extern Uint32 MIL_EventThreadID(void);
#endif


extern void RISCOS_BackgroundTasks(void);

/* The first ticks value of the application */
clock_t start;

void MIL_StartTicks(void)
{
	/* Set first ticks value */
	start = clock();
}

Uint32 MIL_GetTicks (void)
{
	clock_t ticks;

	ticks=clock()-start;


#if CLOCKS_PER_SEC == 1000

	return(ticks);

#elif CLOCKS_PER_SEC == 100

	return (ticks * 10);

#else

	return ticks*(1000/CLOCKS_PER_SEC);

#endif

}

void MIL_Delay (Uint32 ms)
{
    Uint32 now,then,elapsed;
#if !MIL_THREADS_DISABLED
    int is_event_thread;
    if (riscos_using_threads)
    {
       is_event_thread = 0;
       if (MIL_EventThreadID())
       {
          if (MIL_EventThreadID() == MIL_ThreadID()) is_event_thread = 1;
       } else if (MIL_ThreadID() == riscos_main_thread) is_event_thread = 1;
    } else is_event_thread = 1;
#endif

        /*TODO: Next version of Unixlib may allow us to use usleep here */
        /*      for non event threads */

	/* Set the timeout interval - Linux only needs to do this once */
	then = MIL_GetTicks();

	do {
		/* Do background tasks required while sleeping as we are not multithreaded */
#if MIL_THREADS_DISABLED
		RISCOS_BackgroundTasks();
#else
		/* For threaded build only run background tasks in event thread */
		if (is_event_thread) RISCOS_BackgroundTasks();
#endif

		/* Calculate the time interval left (in case of interrupt) */
		now = MIL_GetTicks();
		elapsed = (now-then);
		then = now;
		if ( elapsed >= ms ) {
			break;
		}
		ms -= elapsed;
#if !MIL_THREADS_DISABLED
            /* Need to yield to let other threads have a go */
            if (riscos_using_threads) pthread_yield();
#endif

	} while ( 1 );
}

#if MIL_THREADS_DISABLED

/* Non-threaded version of timer */

int MIL_SYS_TimerInit(void)
{
	return(0);
}

void MIL_SYS_TimerQuit(void)
{
	MIL_SetTimer(0, NULL);
}

int MIL_SYS_StartTimer(void)
{
	timerStart = MIL_GetTicks();

	return(0);
}

void MIL_SYS_StopTimer(void)
{
	/* Don't need to do anything as we use MIL_timer_running
	   to detect if we need to check the timer */
}


void RISCOS_CheckTimer()
{
	if (MIL_timer_running && MIL_GetTicks() - timerStart >= MIL_alarm_interval)
	{
		Uint32 ms;

		ms = MIL_alarm_callback(MIL_alarm_interval);
		if ( ms != MIL_alarm_interval )
		{
			if ( ms )
			{
				MIL_alarm_interval = ROUND_RESOLUTION(ms);
			} else
			{
				MIL_alarm_interval = 0;
				MIL_timer_running = 0;
			}
		}
		if (MIL_alarm_interval) timerStart = MIL_GetTicks();
	}
}

#else

/* Threaded version of timer - based on code for linux */

#include "MIL_thread.h"

/* Data to handle a single periodic alarm */
static int timer_alive = 0;
static MIL_Thread *timer = NULL;

static int RunTimer(void *unused)
{
	while ( timer_alive ) {
		if ( MIL_timer_running ) {
			MIL_ThreadedTimerCheck();
		}
		MIL_Delay(1);
	}
	return(0);
}

/* This is only called if the event thread is not running */
int MIL_SYS_TimerInit(void)
{
	timer_alive = 1;
	timer = MIL_CreateThread(RunTimer, NULL);
	if ( timer == NULL )
		return(-1);
	return(MIL_SetTimerThreaded(1));
}

void MIL_SYS_TimerQuit(void)
{
	timer_alive = 0;
	if ( timer ) {
		MIL_WaitThread(timer, NULL);
		timer = NULL;
	}
}

int MIL_SYS_StartTimer(void)
{
	MIL_SetError("Internal logic error: RISC OS uses threaded timer");
	return(-1);
}

void MIL_SYS_StopTimer(void)
{
	return;
}

#endif /* MIL_THREADS_DISABLED */

#endif /* MIL_TIMER_RISCOS */
