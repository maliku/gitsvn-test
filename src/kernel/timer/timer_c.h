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

/* Useful functions and variables from MIL_timer.c */
#include "MIL_timer.h"

#define ROUND_RESOLUTION(X)	\
	(((X+TIMER_RESOLUTION-1)/TIMER_RESOLUTION)*TIMER_RESOLUTION)

extern int MIL_timer_started;
extern int MIL_timer_running;

/* Data to handle a single periodic alarm */
extern Uint32 MIL_alarm_interval;
extern MIL_TimerCallback MIL_alarm_callback;

/* Set whether or not the timer should use a thread.
   This should be called while the timer subsystem is running.
*/
extern int MIL_SetTimerThreaded(int value);

extern int MIL_TimerInit(void);
extern void MIL_TimerQuit(void);

/* This function is called from the MIL event thread if it is available */
extern void MIL_ThreadedTimerCheck(void);
