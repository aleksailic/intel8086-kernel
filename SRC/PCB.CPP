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
	this->id=PCB::nextId++;
}

PCB::PCB(StackSize stackCapacity,Time quant,Thread* thread,unsigned flags){
		scap=stackCapacity;
		stack=new unsigned[1024];

		stack[stackCapacity-1]=0x200;
		stack[stackCapacity-2]=FP_SEG(PCB::triggerRun);
		stack[stackCapacity-3]=FP_OFF(PCB::triggerRun);

		sp=FP_OFF(stack + stackCapacity - 12);
		ss=FP_SEG(stack + stackCapacity - 12);
		bp=FP_OFF(stack + stackCapacity - 12);

		this->quant=quant;
		this->flags=(flags == 0 ? (quant == 0 ? PCB_UNLIMITED : 0 ) : flags);
		this->myThread=thread;
		this->id=PCB::nextId++;

		cout<<"Kreiran PCB "<< this->id << "\n";
}
PCB* PCB::running=new PCB(); //Glavni tok programa ima svoj PCB i id:0
unsigned PCB::nextId=0;

void PCB::triggerRun(){
	running->myThread->run();
	SET_RFLAG(PCB_FINISHED);
	//nakon sto se zavrsio run dispatchujemo
	dispatch();
}


