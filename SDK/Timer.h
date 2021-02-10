/*! \file
	\brief Countdown timers for MPC555 systems

	The count down timers are based upon the 64-bit timebase counter of the
	MPC555.  The counters are used by first calling WrTimer(), and then later
	calling RdTimer() to determine if the timer has elapsed.*/

///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// 2621 Wasco St.                //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef _TIMER_H
#define _TIMER_H

#include "Types.h"
#include "TimeBase.h"
#include <time.h>

typedef UInt64 Timer_t;	//!< Timer values are stored in 64 bits

#ifdef __cplusplus
extern "C" {
#endif

Timer_t StartElapsedTimer(void);
double ReadElapsedTime(Timer_t Start);
double ReadElapsedTimeEx(Timer_t* pStart);
UInt32 ReadElapsedTime32(Timer_t Start);
UInt64 ReadElapsedTime64(Timer_t Start);

Timer_t WrTimer(double CountDown);
Timer_t WrTimer32(UInt32 CountDown);
Timer_t WrTimer64(UInt64 CountDown);
UInt32 RdTimer(Timer_t Timer);
UInt64 RdTimer64(Timer_t Timer);
double RdTimerRemaining(Timer_t Timer);
BOOL IsTimerElapsed(Timer_t Timer);

#ifndef WIN32
struct tm * gmtime (const time_t *timp);
#endif

#ifdef __cplusplus
}
#endif

#endif // _TIMER_H
