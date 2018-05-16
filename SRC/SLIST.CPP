/*
 * SLIST.CPP
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#include "SLIST.H"
#include "PCB.H";
#include <IOSTREAM.H>

SleepList::SleepList(){
	head=0;
}
void SleepList::add(PCB* pcb){
	PCB_w*el=new PCB_w(pcb);
	PCB_w**itr=&head;
	for (; *itr != 0; itr = &((*itr)->next)) {
		if ((*itr)->pcb->toSleep >= el->pcb->toSleep) { //ex. 3-> [7 5 0 0]
			(*itr)->pcb->toSleep -= el->pcb->toSleep;
			break;
		}else {//ex. 7-> [3 2 1 0 0]
			el->pcb->toSleep -= (*itr)->pcb->toSleep;
		}
	}
	el->next=*itr;
	*itr=el;
}
void SleepList::update(){
	if(head != 0 && head->pcb->toSleep !=0)
		head->pcb->toSleep--;
}
PCB* SleepList::getReady(){
	PCB* toReturn=0;
	if(head){
		if(head->pcb->toSleep == 0){
			PCB_w *toDelete=head;
			toReturn = head->pcb;
			head=head->next;
			delete toDelete; //cistimo memoriju, pcb se nece obrisati jer je pokazivac
		}
	}
	return toReturn;
}

void SleepList::print(){
	cout<<"sl: ";
	for(PCB_w * itr=head;itr!=0;itr=itr->next){
		cout<< itr->pcb->toSleep << '(' << itr->pcb->id << ')' << " | ";
 	}
	cout<<'\n';
}
