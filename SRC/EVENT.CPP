/*
 * EVENT.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "EVENT.H"
#include "KEVENT.H"
#include "IVTENTRY.H"
#include <IOSTREAM.H>

Event::Event(IVTNo ivtNo){
	myImpl=new KernelEv();
	IVTEntry* entry = IVTEntry::fetch(ivtNo);
	if(entry)
		IVTEntry::fetch(ivtNo)->setEvent(this->myImpl);
	else
		cout<<"GRESKA!";

}
Event::~Event(){
	delete myImpl;
}
void Event::signal(){
	myImpl->signal();
}
void Event::wait(){
	myImpl->wait();
}
