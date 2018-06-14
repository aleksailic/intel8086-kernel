/*
 * IVTENTRY.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "IVTENTRY.H"
#include "KEVENT.H"
#include <DOS.H>

IVTEntry::IVTEntry(IVTNo numEntry,pInterrupt routine):newRoutine(routine),numEntry(numEntry),event(0){
	oldRoutine = getvect(numEntry);
	setvect(numEntry, newRoutine);
	IVTEntry::data[numEntry] = this;
}
IVTEntry::~IVTEntry(){
	setvect(numEntry,oldRoutine);
}

IVTEntry* IVTEntry::data[256]={0}; //initialize all to zero
IVTEntry* IVTEntry::fetch(IVTNo id){
	return data[id];
}
void IVTEntry::setEvent(KernelEv* evt){
	event=evt;
}
void IVTEntry::callOld(){
	oldRoutine();
}
void IVTEntry::signal(){
	if(event)//za debugging
		event->signal();
}
