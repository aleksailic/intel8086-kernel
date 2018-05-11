/*
 * PCB.CPP
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#include "PCB.H"
#include <DOS.H>
#include <IOSTREAM.H>

PCB::PCB(){
	this->flags = PCB_STACKLESS | PCB_THREADLESS;
	this->quant = defaultTimeSlice;
	this->myThread = 0; //opasne igre!
	this->id=++PCB::idCount;
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
		this->id=++PCB::idCount;

		cout<<"Kreiran PCB "<< this->id << "\n";
}
PCB* PCB::running=new PCB(); //Ne postoji nit ali mora postojati PCB
unsigned PCB::idCount=0;

void PCB::triggerRun(){
	running->myThread->run();
	//nakon sto se zavrsio run dispatchujemo
	running->flags = PCB_FINISHED;
	dispatch();
}



