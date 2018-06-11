/*
 * THREAD.CPP
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */
#include <DOS.H>
#include "PCB.H"
#include "SCHEDULE.H"
#include "TIMER.H"
#include <IOSTREAM.H>

Thread::Thread(StackSize sz, Time quant){
	myPCB = new PCB(sz,quant,this); //namesta flag automatski
}
void Thread::start(){
	cout<<"U scheduler stavljen PCB "<<myPCB->id <<'\n';
	Scheduler::put(myPCB);
}

void Thread::waitToComplete(){
	myPCB->finished.wait(1);
}

Thread::~Thread(){
	//Ne moze waitToComplete() da se pozove jer se brise vtable, pa run() ne moze da se zavrsi :(
	//waitToComplete(); //cekamo da se zavrsi ako nije, a pozvao se destruktor (po zadatku)
	delete myPCB; //oslobadjamo prostor, u redu jer nece biti u scheduleru posto su tada blokirani prekidi
}


void Thread::sleep(Time toSleep){
	asm cli;
	PCB::running->toSleep=toSleep;
	SET_FLAG(PCB::running->flags,PCB_SLEEPING);
	SET_TFLAG(Timer::CHANGE_CONTEXT);
	Timer::routine();
	asm sti;
}

void dispatch(){
	asm cli;
	SET_TFLAG(Timer::CHANGE_CONTEXT);
	Timer::routine(); //da li se PSW stavlja na stek je pitanje? -Da, to radi kljucna rec interrupt
	asm sti;
}


