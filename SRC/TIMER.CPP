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

static unsigned tsp;
static unsigned tss;
static unsigned tbp;

static PCB* wakeMe=0;
static PCB* nop=PCB::createNOP();

static SleepList sl;
volatile timestamp sysruntime=0;

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
	SET_TFLAG(CHANGE_CONTEXT);
	routine();
}

volatile int Timer::flags=0;
volatile int Timer::count=2; //defaultTimeSlice

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
	if ( !Lock::isSet() && ((Timer::count == 0 && !CHK_RFLAG(PCB_UNLIMITED)) || CHK_TFLAG(CHANGE_CONTEXT))) {
		//cuvamo sp
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
		/*
		cout<<"ctx(";
		if(CHK_TFLAG(TIMER_CHANGE_CONTEXT))
			cout<<"ex";
		else
			cout<<tcount;
		cout<<"):"<<PCB::running->id;
		asm cli;
		*/

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;
		CLR_RFLAG(PCB_RUNNING);

		if(CHK_RFLAG(PCB_SLEEPING))
			sl.add(PCB::running);

		if(!CHK_RFLAG(PCB_FINISHED) && !CHK_RFLAG(PCB_SLEEPING) && !CHK_RFLAG(PCB_BLOCKED) && !CHK_RFLAG(PCB_NOP))
			Scheduler::put((PCB*)PCB::running);
		PCB::running=Scheduler::get();

		if(PCB::running==0)
			PCB::running=nop;

		/*
		cout<<"-> "<<PCB::running->id<<"\n";
		asm cli;
		*/

		SET_STATUS_RFLAG(PCB_RUNNING); //brise ostale statusne stavlja samo running

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		count = PCB::running->quant;

		// restore sp
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}
	//Poziv stare prekidne rutine
	if(!CHK_TFLAG(CHANGE_CONTEXT)) oldRoutine();
	//Vise ne menjamo konteks, ali sacuvaj ostale flegove mozda su relevantne
	CLR_TFLAG(CHANGE_CONTEXT);
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
