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

#ifdef MIL_TIMER_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#include "MIL_timer.h"
#include "../MIL_timer_c.h"

#ifdef _WIN32_WCE
  #error This is WinCE. Please use src/timer/wince/MIL_systimer.c instead.
#endif

#define TIME_WRAP_VALUE	(~(DWORD)0)

/* The first (low-resolution) ticks value of the application */
static DWORD start;

#ifndef USE_GETTICKCOUNT
/* Store if a high-resolution performance counter exists on the system */
static BOOL hires_timer_available;
/* The first high-resolution ticks value of the application */
static LARGE_INTEGER hires_start_ticks;
/* The number of ticks per second of the high-resolution performance counter */
static LARGE_INTEGER hires_ticks_per_second;
#endif

void MIL_StartTicks(void)
{
	/* Set first ticks value */
#ifdef USE_GETTICKCOUNT
	start = GetTickCount();
#else
#if 0 /* Apparently there are problems with QPC on Win2K */
	if (QueryPerformanceFrequency(&hires_ticks_per_second) == TRUE)
	{
		hires_timer_available = TRUE;
		QueryPerformanceCounter(&hires_start_ticks);
	}
	else
#endif
	{
		hires_timer_available = FALSE;
		timeBeginPeriod(1);		/* use 1 ms timer precision */
		start = timeGetTime();
	}
#endif
}

Uint32 MIL_GetTicks(void)
{
	DWORD now, ticks;
#ifndef USE_GETTICKCOUNT
	LARGE_INTEGER hires_now;
#endif

#ifdef USE_GETTICKCOUNT
	now = GetTickCount();
#else
	if (hires_timer_available)
	{
		QueryPerformanceCounter(&hires_now);

		hires_now.QuadPart -= hires_start_ticks.QuadPart;
		hires_now.QuadPart *= 1000;
		hires_now.QuadPart /= hires_ticks_per_second.QuadPart;

		return (DWORD)hires_now.QuadPart;
	}
	else
	{
		now = timeGetTime();
	}
#endif

	if ( now < start ) {
		ticks = (TIME_WRAP_VALUE-start) + now;
	} else {
		ticks = (now - start);
	}
	return(ticks);
}

void MIL_Delay(Uint32 ms)
{
	Sleep(ms);
}

/* Data to handle a single periodic alarm */
static UINT timerID = 0;

static void CALLBACK HandleAlarm(UINT uID,  UINT uMsg, DWORD_PTR dwUser,
						DWORD_PTR dw1, DWORD_PTR dw2)
{
	MIL_ThreadedTimerCheck();
}


int MIL_SYS_TimerInit(void)
{
	MMRESULT result;

	/* Set timer resolution */
	result = timeBeginPeriod(TIMER_RESOLUTION);
	if ( result != TIMERR_NOERROR ) {
		MIL_SetError("Warning: Can't set %d ms timer resolution",
							TIMER_RESOLUTION);
	}
	/* Allow 10 ms of drift so we don't chew on CPU */
	timerID = timeSetEvent(TIMER_RESOLUTION,1,HandleAlarm,0,TIME_PERIODIC);
	if ( ! timerID ) {
		MIL_SetError("timeSetEvent() failed");
		return(-1);
	}
	return(MIL_SetTimerThreaded(1));
}

void MIL_SYS_TimerQuit(void)
{
	if ( timerID ) {
		timeKillEvent(timerID);
	}
	timeEndPeriod(TIMER_RESOLUTION);
}

int MIL_SYS_StartTimer(void)
{
	MIL_SetError("Internal logic error: Win32 uses threaded timer");
	return(-1);
}

void MIL_SYS_StopTimer(void)
{
	return;
}

#endif /* MIL_TIMER_WIN32 */
