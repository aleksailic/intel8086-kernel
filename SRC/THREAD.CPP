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
#include "SYS.H"

Thread::Thread(StackSize sz, Time quant){
	sys_thread_create(sz,quant,this,&id);
}
void Thread::start(){
	sys_thread_start(id);
}

void Thread::waitToComplete(){
	sys_thread_join(id);
}

Thread::~Thread(){
	//Ne moze waitToComplete() da se pozove jer se brise vtable, pa run() ne moze da se zavrsi :(
	//waitToComplete(); //cekamo da se zavrsi ako nije, a pozvao se destruktor (po zadatku)
	//sys_thread_delete(id);
	//delete myPCB; //oslobadjamo prostor, u redu jer nece biti u scheduleru posto su tada blokirani prekidi
}


void Thread::sleep(Time toSleep){
	sys_sleep(toSleep);
}

void dispatch(){
	sys_dispatch();
}


