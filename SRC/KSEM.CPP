/*
 * SEM.CPP
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#include "KSEM.H"
#include "PCB.H"
#include <IOSTREAM.H>
#include "SCHEDULE.H"
#include "KTHREAD.H"

KernelSem::KernelSem(int initValue){
	val=initValue;
	mutex=0;
}
void KernelSem::lock(){
	asm cli; //mask interrupts
	while(mutex); //uposleno cekamo dok nega druga nit ima bravu, ovo je kratkog trajanja pa je opravdano
	mutex=1;
}
void KernelSem::unlock(){
	mutex=0;
	asm sti; //unmask interrupts
}
void KernelSem::signal(){
	lock();
	if(val++<0)
		unblock();
	unlock();
}
int KernelSem::wait(int toBlock){
	lock();
	if(--val<0){
		if(toBlock) block();
		else val++;
	}
	unlock();
	return toBlock ? (val<=0 ? 1 : 0) : (val<=0 ? -1 : 0);
}
void KernelSem::block(){
	//cout<<"sem blocking: "<<PCB::running->id<<endl;
	SET_KFLAG(PCB_BLOCKED);
	blocked.add(KernelThread::blocked);
	mutex=0; //promenom konteksta nikada ne dolazi do unlocka, zato mutex mora da se reseta rucno
	sys_dispatch();
}
void KernelSem::unblock(){
	PCB* pcb = blocked.get();
	//cout<<"sem unblocking: "<<pcb->id<<endl;
	if(pcb) //nepotrbno ali dok smo u "debugging" modu ok
		Scheduler::put(pcb);
}
int KernelSem::getVal()const{
	return val;
}
void KernelSem::setVal(int value){
	lock();
	val=value;
	unlock();
}
