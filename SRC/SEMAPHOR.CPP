/*
 * SEMAPHOR.CPP
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */
#include "SEMAPHOR.H"
#include "SYS.H"

Semaphore::Semaphore(int initValue){
	sys_sem_create(&id,initValue);
}
int Semaphore::wait(int toBlock){
	int retVal;
	sys_sem_wait(id,toBlock,&retVal);
	return retVal;
}
void Semaphore::signal(){
	sys_sem_signal(id);
}
int Semaphore::val()const {
	int val;
	sys_sem_val(id,&val);
	return val;
}
Semaphore::~Semaphore(){
	sys_sem_destroy(id);
}


