/*
 * PCB.CPP
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#include "PCB.H"
#include <DOS.H>
#include <IOSTREAM.H>


PCB::PCB(StackSize stackCapacity,Time quant,Thread* thread,unsigned flags,int id):finished(0){
	this->id= (id<0 ? PCB::nextId++ : id);
	this->myThread=thread;
	this->flags=flags;
	this->quant=quant;
	if(!thread)
		SET_FLAG(this,PCB_THREADLESS);
	if(!quant)
		SET_FLAG(this,PCB_UNLIMITED);
	if(!stackCapacity)
		SET_FLAG(this,PCB_STACKLESS);
	else{
		scap=stackCapacity;
		stack=new unsigned[1024];

		stack[stackCapacity-1]=0x200;
		stack[stackCapacity-2]=FP_SEG(PCB::triggerRun);
		stack[stackCapacity-3]=FP_OFF(PCB::triggerRun);

		sp=FP_OFF(stack + stackCapacity - 12);
		ss=FP_SEG(stack + stackCapacity - 12);
		bp=FP_OFF(stack + stackCapacity - 12);
	}
	//cout<<"Kreiran PCB "<< this->id << "\n";
}
PCB* PCB::running=new PCB(0,defaultTimeSlice,0,PCB_RUNNING,1); //Glavni tok programa ima svoj PCB i id:0
unsigned PCB::nextId=2;

void PCB::triggerRun(){
	running->myThread->run();
	asm cli; // mere predostroznosti
	running->finished.signal();
	SET_RFLAG(PCB_FINISHED);
	dispatch();
	asm sti;
}

static void loopAeterna(){
	while(1);
}

PCB* PCB::createNOP(){
	PCB*nop=new PCB(24,1,0,PCB_NOP);
	nop->stack[nop->scap - 2]=FP_SEG(loopAeterna);
	nop->stack[nop->scap - 3]=FP_OFF(loopAeterna);
	return nop;
}



