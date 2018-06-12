/*
 * TIMER.CPP
 *
 *  Created on: May 12, 2018
 *      Author: OS1
 */

#include <IOSTREAM.H>
#include <DOS.H>
#include "TIMER.H"
#include "SLIST.H"


static PCB* wakeMe=0;
static PCB* nop=PCB::createNOP();

static SleepList sl;
volatile timestamp sysruntime=0;

unsigned tss=0,tsp=0,tcx=0,tdx=0,tbp=0;
extern void tick();

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
}
void Timer::changeContext(){
	SET_TFLAG(TIMER_CHANGE_CONTEXT);
	routine();
}

volatile int Timer::flags=0;
volatile int Timer::count=2; //defaultTimeSlice

//- Pomocni makroi za debagovanje rutine -
#define PRINTOLD()\
	cout<<"ctx(";\
	if(CHK_TFLAG(CHANGE_CONTEXT))\
		cout<<"ex";\
	else\
		cout<<count;\
	cout<<"):"<<PCB::running->id;\
	asm cli;

#define PRINTNEW()\
	cout<<"-> "<<PCB::running->id<<"\n";\
	asm cli;
//----------------------------------------

void interrupt Timer::routine(...){
	if(!Timer::flags){ //nema eksplicitnog poziva
		Timer::count--;
		sysruntime++;
		//tick();
		sl.update();
		while(wakeMe=sl.getReady()){
			CLR_FLAG(wakeMe,PCB_SLEEPING);
			Scheduler::put(wakeMe);
		}
	}
	if ( !Lock::isSet() && ((Timer::count == 0 && !CHK_RFLAG(PCB_UNLIMITED)) || CHK_TFLAG(TIMER_CHANGE_CONTEXT))) {
		STORE_REGS(PCB::running);

		//PRINTOLD()

		CLR_RFLAG(PCB_RUNNING);

		if(CHK_RFLAG(PCB_SLEEPING))
			sl.add(PCB::running);

		if(!CHK_RFLAG(PCB_FINISHED) && !CHK_RFLAG(PCB_SLEEPING) && !CHK_RFLAG(PCB_BLOCKED) && !CHK_RFLAG(PCB_NOP) && !CHK_RFLAG(PCB_KERNEL))
			Scheduler::put((PCB*)PCB::running);
		PCB::running=Scheduler::get();

		if(PCB::running==0)
			PCB::running=nop;

		//PRINTNEW()

		SET_STATUS_RFLAG(PCB_RUNNING); //brise ostale statusne stavlja samo running

		count = PCB::running->quant;
		RESTORE_REGS(PCB::running);
	}
	//Poziv stare prekidne rutine
	if(!CHK_TFLAG(TIMER_CHANGE_CONTEXT)) oldRoutine();
	//Vise ne menjamo konteks, ali sacuvaj ostale flegove mozda su relevantne
	CLR_TFLAG(TIMER_CHANGE_CONTEXT);
}

unsigned char Lock::val=0;
void Lock::set(){
	val=1;
}
void Lock::unset(){
	val=0;
}
unsigned char Lock::isSet(){
	return val;
}
