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

#ifdef MIL_TIMER_MACOS

#include <Types.h>
#include <Timer.h>
#include <OSUtils.h>
#include <Gestalt.h>
#include <Processes.h>

#include <LowMem.h>

#include "MIL_timer.h"
#include "../MIL_timer_c.h"

#include "FastTimes.h"

#if TARGET_API_MAC_CARBON
#define NewTimerProc NewTimerUPP
#endif

#define MS_PER_TICK	(1000.0/60.0)		/* MacOS tick = 1/60 second */


#define kTwoPower32     (4294967296.0)          /* 2^32 */

static double start_tick;
static int    is_fast_inited = 0;

void MIL_StartTicks(void)
{
        if ( ! is_fast_inited )     // important to check or FastTime may hang machine!
            MIL_SYS_TimerInit();

        start_tick = FastMicroseconds();
}

Uint32 MIL_GetTicks(void)
{
        
        if ( ! is_fast_inited )
            MIL_SYS_TimerInit();
         
        return FastMilliseconds();
}

void MIL_Delay(Uint32 ms)
{
        Uint32 stop, now;

        stop = MIL_GetTicks() + ms;
        do {
            #if TARGET_API_MAC_CARBON
                MPYield();
            #else
                SystemTask();
            #endif

                now = MIL_GetTicks();

        } while ( stop > now );
}

/*
void MIL_StartTicks(void)
{
	// FIXME: Should we implement a wrapping algorithm, like Win32? 
}

Uint32 MIL_GetTicks(void)
{
	UnsignedWide ms;
	
	Microseconds (&ms);
	
	return ( ms.lo / 1000 );
}

void MIL_Delay(Uint32 ms)
{
	
	UnsignedWide microsecs;
	UInt32       stop;
	
	Microseconds (&microsecs);
	
	stop = microsecs.lo + (ms * 1000);
	
	while ( stop > microsecs.lo ) {
	
	   SystemTask ();
	   
	   Microseconds (&microsecs);
	}

}*/
 
/* Data to handle a single periodic alarm */
typedef struct _ExtendedTimerRec
{
	TMTask		     tmTask;
	ProcessSerialNumber  taskPSN;
} ExtendedTimerRec, *ExtendedTimerPtr;

static ExtendedTimerRec gExtendedTimerRec;


int MIL_SYS_TimerInit(void)
{
	FastInitialize ();
	is_fast_inited = 1;
	
	return(0);
}

void MIL_SYS_TimerQuit(void)
{
	/* We don't need a cleanup? */
	return;
}

/* Our Stub routine to set up and then call the real routine. */
pascal void TimerCallbackProc(TMTaskPtr tmTaskPtr)
{
	Uint32 ms;

	WakeUpProcess(&((ExtendedTimerPtr) tmTaskPtr)->taskPSN);

	ms = MIL_alarm_callback(MIL_alarm_interval);
	if ( ms ) {
		MIL_alarm_interval = ROUND_RESOLUTION(ms);
		PrimeTime((QElemPtr)&gExtendedTimerRec.tmTask,
		          MIL_alarm_interval);
	} else {
		MIL_alarm_interval = 0;
	}
}

int MIL_SYS_StartTimer(void)
{
	/*
	 * Configure the global structure that stores the timing information.
	 */
	gExtendedTimerRec.tmTask.qLink = NULL;
	gExtendedTimerRec.tmTask.qType = 0;
	gExtendedTimerRec.tmTask.tmAddr = NewTimerProc(TimerCallbackProc);
	gExtendedTimerRec.tmTask.tmCount = 0;
	gExtendedTimerRec.tmTask.tmWakeUp = 0;
	gExtendedTimerRec.tmTask.tmReserved = 0;
	GetCurrentProcess(&gExtendedTimerRec.taskPSN);

	/* Install the task record */
	InsXTime((QElemPtr)&gExtendedTimerRec.tmTask);

	/* Go! */
	PrimeTime((QElemPtr)&gExtendedTimerRec.tmTask, MIL_alarm_interval);
	return(0);
}

void MIL_SYS_StopTimer(void)
{
	RmvTime((QElemPtr)&gExtendedTimerRec.tmTask);
}

#endif /* MIL_TIMER_MACOS */
