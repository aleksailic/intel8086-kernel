/*
 * TIMER.CPP
 *
 *  Created on: May 12, 2018
 *      Author: OS1
 */

#include <IOSTREAM.H>
#include <DOS.H>
#include "TIMER.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "SLIST.H"
#include "SYS.H"

const int Timer::routineNo = 0x8;
pInterrupt Timer::oldRoutine=0;
void Timer::changeRoutine(){
	oldRoutine = getvect(routineNo);
	setvect(routineNo, routine);
}
void Timer::restoreRoutine(){
	setvect(routineNo,oldRoutine);
}
void Timer::create(){
	changeRoutine();
}
void Timer::destroy(){
	restoreRoutine();
	delete sleeplist;
}

volatile int Timer::count=2; //defaultTimeSlice
volatile timestamp sysruntime=0;
extern void tick();
static PCB* wakeMe=0;
SleepList* Timer::sleeplist = new SleepList();

void interrupt Timer::routine(...){
	Timer::count--;
	sysruntime++;
	//tick();
	cout<<"PCB: "<<PCB::running->id<<", TIMER COUNT: !"<<Timer::count<<endl;
	sleeplist->update();
	while(wakeMe=sleeplist->getReady()){
		CLR_FLAG(wakeMe,PCB_SLEEPING);
		Scheduler::put(wakeMe);
	}
	if ( !Lock::isSet() && (Timer::count <= 0 && !CHK_RFLAG(PCB_UNLIMITED)))
		sys_dispatch();
	oldRoutine();
}
