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
	//asm sti;
	while(!CHK_FLAG(myPCB->flags,PCB_FINISHED));
}

Thread::~Thread(){
	waitToComplete(); //cekamo da se zavrsi ako nije, a pozvao se destruktor (po zadatku)
	delete myPCB; //oslobadjamo prostor, u redu jer nece biti u scheduleru posto su tada blokirani prekidi
}


void Thread::sleep(Time toSleep){
	if(PCB::running->id == 0){
		timestamp endTime= toSleep+sysruntime;
		while(sysruntime < endTime); //busy wait, opravdan jer se main stavlja u sleep samo u okolnostima kad nema drugih niti
	}else{
		asm cli;
		PCB::running->toSleep=toSleep;
		SET_FLAG(PCB::running->flags,PCB_SLEEPING);
		SET_TFLAG(TIMER_CHANGE_CONTEXT);
		timer();
		asm sti;
	}
}

void dispatch(){
	asm cli;
	SET_TFLAG(TIMER_CHANGE_CONTEXT);
	timer(); //da li se PSW stavlja na stek je pitanje?
	asm sti;
}
