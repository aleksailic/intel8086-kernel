/*
 * PCB.H
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */


#ifndef PCB_H_
#define PCB_H_

struct PCB{
	unsigned sp;
	unsigned ss;
	unsigned zavrsio;
	int kvant;
};

PCB *p[3];
volatile PCB* running;

volatile int nextThread = 2;

PCB* getNextPCBToExecute(){
	if (nextThread == 1)
		nextThread = 2;
	else nextThread = 1;
	if (p[nextThread]->zavrsio){
		if (nextThread == 1)
			nextThread = 2;
		else nextThread = 1;
		if (p[nextThread]->zavrsio)
			nextThread = 0;
	}
	return p[nextThread];
}

/*
#include <DOS.H>

#define MASK(n,off) (1u<<(n+off)-1) & (~0<<off)
#define SET_STATUS_FLAGS(n) flags = (flags & (~15)) + n

enum{
	PCB_RUNNING = 0x1,
	PCB_BLOCKED = 0x2,
	PCB_FINISHED = 0x4,
	PCB_UNLIMITED= 0x10
};

struct PCB{
	friend class Thread;
	unsigned flags;
	unsigned ss;
	unsigned sp;
	unsigned long scap; //kapacitet steka, pazimo na stack overflow
	unsigned bp;
	unsigned* stack;
	Time quant;
	Thread* myThread;

	PCB(unsigned long stackCapacity,Time quant,Thread* thread,unsigned flags=0){
		this->scap=stackCapacity;
		this->stack=new unsigned[stackCapacity];
		this->quant=quant;
		this->flags=(flags == 0 ? (quant == 0 ? PCB_UNLIMITED : 0 ) : flags);
		this->myThread=thread;
	}
	static PCB* running;
	static void triggerRun();
};

void PCB::triggerRun(){
	running->myThread->run();
	//nakon sto se zavrsio run dispatchujemo
	running->flags |= PCB_FINISHED;
	dispatch();
}

*/


#endif /* PCB_H_ */
