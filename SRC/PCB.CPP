/*
 * PCB.CPP
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#include "PCB.H"
#include <DOS.H>
#include <IOSTREAM.H>

PCB::PCB(StackSize stackCapacity,Time quant,Thread* thread,unsigned flags,int id,void(*fn)()):finished(0){
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
		stack=new unsigned[scap];

		stack[stackCapacity-1]=0x200;
		stack[stackCapacity-2]=FP_SEG(fn);
		stack[stackCapacity-3]=FP_OFF(fn);

		sp=FP_OFF(stack + stackCapacity - 12);
		ss=FP_SEG(stack + stackCapacity - 12);
		bp=FP_OFF(stack + stackCapacity - 12);
	}
	//cout<<"Kreiran PCB "<< this->id << "\n";
}

PCB* PCB::running=new PCB(0,defaultTimeSlice,0,PCB_RUNNING,1); //Glavni tok programa ima svoj PCB i id:1
unsigned PCB::nextId=3;

void PCB::triggerRun(){
	running->myThread->run();
	//cout<<"JA("<<running->id-3<<") TO VOLIM!!!";
	asm cli; // mere predostroznosti
	running->finished.signal();
	SET_RFLAG(PCB_FINISHED);
	dispatch();
	asm sti;
}

static void loopAeterna(){
	while(1);
}

PCB* PCB::nop=new PCB(1024,1,0,PCB_NOP,2,loopAeterna);
