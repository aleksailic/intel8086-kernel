/*
 * PCB.H
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */


#ifndef PCB_H_
#define PCB_H_

#define MASK(n,off) (1u<<(n+off)-1) & (~0<<off)

#define CLR_RFLAG(f) PCB::running->flags &= ~f
#define CHK_RFLAG(f) (PCB::running->flags & f)
#define SET_RFLAG(f) PCB::running->flags |= f
#define SET_STATUS_RFLAG(f) PCB::running->flags = (PCB::running->flags & (~15)) + f

#define CLR_FLAG(var,f) var &= ~f
#define CHK_FLAG(var,f) (var & f)
#define SET_FLAG(var,f) var |= f
#define SET_STATUS_FLAG(var,f) var = (var & (~15)) + f

#include "THREAD.H"

enum{
	PCB_RUNNING = 0x1,
	PCB_BLOCKED = 0x2,
	PCB_SLEEPING = 0x4,
	PCB_FINISHED = 0x8,
	PCB_UNLIMITED = 0x10,
	PCB_STACKLESS = 0x20,
	PCB_THREADLESS = 0x40
};

class PCB{
public:
	friend class Thread;
	unsigned flags;
	unsigned ss;
	unsigned sp;
	unsigned long scap;
	unsigned bp;
	unsigned* stack;
	unsigned id;
	Time quant;
	Time toSleep;

	Thread* myThread;

	PCB();
	PCB(StackSize stackCapacity, Time quant,Thread* thread, unsigned flags=0);
	static PCB* running;
	static void triggerRun();
	static unsigned lastId;
};

#endif /* PCB_H_ */
