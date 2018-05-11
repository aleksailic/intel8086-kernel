/*
 * PCB.CPP
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#include "PCB.H"

PCB::PCB(){
	this->flags = PCB_STACKLESS | PCB_THREADLESS;
	this->quant = 20; //defaultTimeSlice ovde!
	this->myThread = 0; //opasne igre!
}

PCB::PCB(StackSize stackCapacity,Time quant,Thread* thread,unsigned flags=0){
		this->scap=stackCapacity;
		this->stack=new unsigned[stackCapacity];
		this->quant=quant;
		this->flags=(flags == 0 ? (quant == 0 ? PCB_UNLIMITED : 0 ) : flags);
		this->myThread=thread;
}
PCB* PCB::running=0; //nullptr for pcb running initally

void PCB::triggerRun(){
	running->myThread->run();
	//nakon sto se zavrsio run dispatchujemo
	running->flags |= PCB_FINISHED;
	dispatch();
}

