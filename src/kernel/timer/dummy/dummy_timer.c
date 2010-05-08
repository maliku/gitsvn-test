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

#if defined(MIL_TIMER_DUMMY) || defined(MIL_TIMERS_DISABLED)

#include "MIL_timer.h"
#include "../MIL_timer_c.h"

void MIL_StartTicks(void)
{
}

Uint32 MIL_GetTicks (void)
{
	MIL_Unsupported();
	return 0;
}

void MIL_Delay (Uint32 ms)
{
	MIL_Unsupported();
}

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
	MIL_SetError("Internal logic error: threaded timer in use");
	return(-1);
}

void MIL_SYS_StopTimer(void)
{
	return;
}

#endif /* MIL_TIMER_DUMMY || MIL_TIMERS_DISABLED */
