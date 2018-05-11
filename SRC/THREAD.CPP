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
	Scheduler::put(PCB::running);
}

void Thread::waitToComplete(){
	if(!(myPCB->flags & PCB_FINISHED)){ //ako nije zavrseno
		//cekaj da se zavrsi: busy wait (ne)? interrupt (mozda + neki queue)? dogadjaji (bingo!)
	}
}

Thread::~Thread(){
	waitToComplete(); //cekamo da se zavrsi ako nije, a pozvao se destruktor (po zadatku)
	myPCB->flags=PCB_FINISHED;
	//delete myPCB; //oslobadjamo prostor
}


void Thread::sleep(Time toSleep){

}

void dispatch(){
	asm cli;
	timer_flags=TIMER_CHANGE_CONTEXT;
	timer();
	asm sti;
}
