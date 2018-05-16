/*
 * FIFOLIST.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */


#include "FLIST.H"
#include "PCB.H"
#include <IOSTREAM.H>

FList::FList():first(0),last(0){}
void FList::add(PCB* pcb){
	last = (first ? last->next : first) = new PCB_w(pcb);
}
PCB* FList::get(){
	if(empty())
		return 0;

	PCB*toReturn = first->pcb;
	PCB_w*toDelete = first;
	first=first->next;
	delete toDelete;
	return toReturn;
}
int FList::empty(){
	return (first==0 ? 1 : 0);
}
void FList::print(){
	cout<<"fl: ";
	for(PCB_w * itr = first;itr!=0;itr=itr->next)
		cout<< "pcb"<<itr->pcb->id<<"|";
	cout<<endl;
}
