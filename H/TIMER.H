/*
 * TIMER.H
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "PCB.H"
#include "SCHEDULE.H"

typedef unsigned long timestamp;
typedef void interrupt (*pInterrupt)(...);

#define CHK_TFLAG(f) (Timer::flags & f)
#define CLR_TFLAG(f) Timer::flags &= ~f
#define SET_TFLAG(f) Timer::flags |= f

extern volatile timestamp sysruntime;

class Timer{
	static pInterrupt oldRoutine;
	static const int routineNo;

	static volatile int count;

	static void changeRoutine();
	static void restoreRoutine();
public:
	static volatile int flags;
	enum{
		CHANGE_CONTEXT = 0x1
	};
	static void create();
	static void interrupt routine(...);
	static void destroy();
};


#endif /* TIMER_H_ */
