/*
 * KEVENT.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "KEVENT.H"
#include "PCB.H"
#include <IOSTREAM.H>
#include "SCHEDULE.H"
#include "KTHREAD.H"

KernelEv::KernelEv():val(0),mutex(0),status(0){
	myPCB=KernelThread::blocked;
}
void KernelEv::wait(){
	lock();
	if(KernelThread::blocked==myPCB){
		if(val==0) block();
		else val=0;
	}
	unlock();
}
void KernelEv::signal(){
	lock();
	if(val==0 && status==1)
		unblock();
	val=1;
	unlock();
}
void KernelEv::block(){
	//cout<<"ev blocking: "<<PCB::running->id<<endl;
	status=1;
	mutex=0; //promenom konteksta nikada ne dolazi do unlocka, zato mutex mora da se reseta rucno
	sys_dispatch();
}
void KernelEv::unblock(){
	//cout<<"ev unblock: "<<myPCB->id<<endl;
	status=0;
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
