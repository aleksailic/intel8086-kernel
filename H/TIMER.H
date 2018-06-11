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
	static void interrupt routine(...);
	Timer();
public:
	static volatile int flags;
	enum{
		CHANGE_CONTEXT = 0x1,
		SET_KERNEL=0x2,
		SET_SAVED=0x4
	};
	static void create();
	static void destroy();
	static inline void changeContext();
};

class Lock{
	static unsigned char val;
	Lock();
public:
	static void set();
	static void unset();
	static unsigned char isSet(); //ne postoji bool
};


#endif /* TIMER_H_ */
