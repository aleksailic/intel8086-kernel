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
	while(!CHK_FLAG(myPCB->flags,PCB_FINISHED)){ //ako nije zavrseno
		//cekaj da se zavrsi: busy wait (ne)? interrupt (mozda + neki queue)? dogadjaji (bingo!)
	}
}

Thread::~Thread(){
	waitToComplete(); //cekamo da se zavrsi ako nije, a pozvao se destruktor (po zadatku)
	SET_FLAG(myPCB->flags,PCB_FINISHED);
	//delete myPCB; //oslobadjamo prostor
}


void Thread::sleep(Time toSleep){
	asm cli;
	PCB::running->toSleep=toSleep;
	SET_FLAG(PCB::running->flags,PCB_SLEEPING);
	SET_TFLAG(TIMER_CHANGE_CONTEXT);
	timer();
	asm sti;
}

void dispatch(){
	asm cli;
	SET_TFLAG(TIMER_CHANGE_CONTEXT);
	timer(); //da li se PSW stavlja na stek je pitanje?
	asm sti;
}
