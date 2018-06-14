/*
 * THREAD.CPP
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */
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
	sys_thread_destroy(id);
}

void Thread::sleep(Time toSleep){
	sys_sleep(toSleep);
}

void dispatch(){
	sys_dispatch();
}


