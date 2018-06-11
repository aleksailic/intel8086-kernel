/*
 * PCB.H
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */


#ifndef PCB_H_
#define PCB_H_

#define MASK(n,off) (1u<<(n+off)-1) & (~0<<off)

#define CLR_FLAG(pcb,f) pcb->flags &= ~f
#define CHK_FLAG(pcb,f) (pcb->flags & f)
#define SET_FLAG(pcb,f) pcb->flags |= f
#define SET_STATUS_FLAG(pcb,f) pcb->flags = (pcb->flags & (~15)) + f

#define CLR_RFLAG(f) CLR_FLAG(PCB::running,f)
#define CHK_RFLAG(f) CHK_FLAG(PCB::running,f)
#define SET_RFLAG(f) SET_FLAG(PCB::running,f)
#define SET_STATUS_RFLAG(f) SET_STATUS_FLAG(PCB::running,f)

#include "THREAD.H"
#include "KSEM.H"
enum{
//  -  Status flags  -
	PCB_RUNNING = 0x1,
	PCB_BLOCKED = 0x2,
	PCB_SLEEPING = 0x4,
	PCB_FINISHED = 0x8,
//  ------------------

	PCB_UNLIMITED = 0x10,
	PCB_KERNEL = 0x20,
	PCB_THREADLESS = 0x40,
	PCB_NOP = 0x80,

	PCB_STACKLESS = 0x100
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
	KernelSem finished;

	PCB(StackSize stackCapacity=0,Time quant=defaultTimeSlice,Thread* thread=0,unsigned flags=0,int id=-1);

	static PCB* running;
	static void triggerRun();
	static unsigned nextId;
	static PCB* createNOP();
};

#endif /* PCB_H_ */
