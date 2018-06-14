/*
 * EVENT.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "EVENT.H"
#include "SYS.H"

Event::Event(IVTNo ivtNo){
	sys_event_create(&id,ivtNo);
}
Event::~Event(){
	sys_event_destroy(id);
}
void Event::signal(){
	sys_event_signal(id);
}
void Event::wait(){
	sys_event_wait(id);
}
