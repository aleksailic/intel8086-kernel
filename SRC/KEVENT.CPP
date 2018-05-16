/*
 * KEVENT.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "KEVENT.H"
#include "PCB.H"
#include "TIMER.H"
#include <IOSTREAM.H>

KernelEv::KernelEv():val(0),mutex(0){
	myPCB=PCB::running;
}
void KernelEv::wait(){
	lock();
	if(PCB::running==myPCB){
		if(val==0) block();
		else val=0;
	}
	unlock();
}
void KernelEv::signal(){
	lock();
	if(val==0 && CHK_FLAG(myPCB->flags,PCB_BLOCKED))
		unblock();
	val=1;
	unlock();
}
void KernelEv::block(){
	cout<<"ev blocking: "<<PCB::running->id<<endl;
	SET_RFLAG(PCB_BLOCKED);
	SET_TFLAG(TIMER_CHANGE_CONTEXT);
	mutex=0; //promenom konteksta nikada ne dolazi do unlocka, zato mutex mora da se reseta rucno
	timer();
}
void KernelEv::unblock(){
	cout<<"ev unblock: "<<myPCB->id<<endl;
	CLR_FLAG(myPCB->flags,PCB_BLOCKED);
	Scheduler::put(myPCB);
}

void KernelEv::lock(){
	asm cli; //mask interrupts
	while(mutex); //uposleno cekamo dok nega druga nit ima bravu, ovo je kratkog trajanja pa je opravdano
	mutex=1;
}
void KernelEv::unlock(){
	mutex=0;
	asm sti; //unmask interrupts
}
