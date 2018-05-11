/*
 * PCB.CPP
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#include "PCB.H"
#include <DOS.H>

PCB::PCB(){
	this->flags = PCB_STACKLESS | PCB_THREADLESS;
	this->quant = 20; //defaultTimeSlice ovde!
	this->myThread = 0; //opasne igre!
}

PCB::PCB(StackSize stackCapacity,Time quant,Thread* thread,unsigned flags){
		scap=stackCapacity;
		stack=new unsigned[1024];

		stack[1023]=0x200;
		stack[1022]=FP_SEG(PCB::triggerRun);
		stack[1021]=FP_OFF(PCB::triggerRun);

		sp=FP_OFF(stack + 1012);
		ss=FP_SEG(stack + 1012);

		this->quant=quant;
		this->flags=(flags == 0 ? (quant == 0 ? PCB_UNLIMITED : 0 ) : flags);
		this->myThread=thread;
}
PCB* PCB::running=new PCB(); //Ne postoji nit ali mora postojati PCB

void PCB::triggerRun(){
	running->myThread->run();
	//nakon sto se zavrsio run dispatchujemo
	running->flags = PCB_FINISHED;
	dispatch();
}

