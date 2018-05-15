/*
 * SEMAPHOR.CPP
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */
#include "SEMAPHOR.H"
#include "KSEM.H"

Semaphore::Semaphore(int initValue){
	myImpl=new KernelSem(initValue);
}
int Semaphore::wait(int toBlock){
	return myImpl->wait(toBlock);
}
void Semaphore::signal(){
	myImpl->signal();
}
int Semaphore::val()const {
	return myImpl->getVal();
}
Semaphore::~Semaphore(){
	delete myImpl;
}


