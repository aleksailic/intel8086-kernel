/*
 * SEM.CPP
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#include "SEM.H"
#include "PCB.H"
#include "TIMER.H"

static unsigned tsp;
static unsigned tss;
static unsigned tbp;

Semaphore::Semaphore(int initValue){
	val=initValue;
	mutex=0;
}
void Semaphore::lock(){
	asm cli; //mask interrupts
	while(mutex); //uposleno cekamo dok nega druga nit ima bravu, ovo je kratkog trajanja pa je opravdano
	mutex=1;
}
void Semaphore::unlock(){
	mutex=0;
	asm sti; //unmask interrupts
}
void Semaphore::signal(){
	lock();
	if(val++<0)
		unblock();
	unlock();
}
void Semaphore::wait(){
	lock();
	if(--val<0)
		block();
	unlock();
}
void Semaphore::block(){
	blocked.put(PCB::running);
	SET_RFLAG(PCB_BLOCKED);
	SET_TFLAG(TIMER_CHANGE_CONTEXT);
	timer();
}
void Semaphore::unblock(){
	PCB* pcb = blocked.get();
	if(pcb) //nepotrbno ali dok smo u "debugging" modu ok
		Scheduler::put(pcb);
}
