/*
 * TIMER.H
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef unsigned long timestamp;
typedef void interrupt (*pInterrupt)(...);

#define CHK_TFLAG(f) (Timer::flags & f)
#define CLR_TFLAG(f) Timer::flags &= ~f
#define SET_TFLAG(f) Timer::flags |= f

#define STORE_REGS(pcb)\
	asm mov tsp, sp;\
	asm mov tss, ss;\
	asm mov tbp, bp;\
	(pcb)->sp = tsp;\
	(pcb)->ss = tss;\
	(pcb)->bp = tbp
#define RESTORE_REGS(pcb)\
	tsp = (pcb)->sp;\
	tss = (pcb)->ss;\
	tbp = (pcb)->bp;\
	asm mov sp, tsp;\
	asm mov ss, tss;\
	asm mov bp, tbp


extern volatile timestamp sysruntime;

class SleepList;
class Timer{
	static pInterrupt oldRoutine;
	static const int routineNo;

	static void changeRoutine();
	static void restoreRoutine();
	static void interrupt routine(...);
	Timer();
public:
	static volatile int count;
	static SleepList* sleeplist;
	static volatile int flags;
	enum{
		CHANGE_CONTEXT = 0x1
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
