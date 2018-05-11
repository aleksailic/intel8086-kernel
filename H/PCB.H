/*
 * PCB.H
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */


#ifndef PCB_H_
#define PCB_H_

#define MASK(n,off) (1u<<(n+off)-1) & (~0<<off)
#define SET_STATUS_FLAGS(n) flags = (flags & (~15)) + n

#include "THREAD.H"

enum{
	PCB_RUNNING = 0x1,
	PCB_BLOCKED = 0x2,
	PCB_FINISHED = 0x4,
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
	Time quant;

	Thread* myThread;

	PCB();
	PCB(StackSize stackCapacity, Time quant,Thread* thread, unsigned flags);
	static PCB* running;
	static void triggerRun();
};


#endif /* PCB_H_ */
