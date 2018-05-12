/*
 * TIMER.CPP
 *
 *  Created on: May 12, 2018
 *      Author: OS1
 */

#include <IOSTREAM.H>
#include "TIMER.H"

volatile int timer_flags=0;
volatile int timer_count=2;

static unsigned tsp;
static unsigned tss;
static unsigned tbp;

void interrupt timer(){	// prekidna rutina
	cout<<"TIMER: " << timer_count<< '\n';
	if (!(timer_flags & TIMER_CHANGE_CONTEXT)) timer_count--;
	if (timer_count == 0 || (timer_flags & TIMER_CHANGE_CONTEXT)) {
		//cuvamo sp
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;
		PCB::running->flags |= PCB_BLOCKED;

		if(!(PCB::running->flags & PCB_FINISHED))
			Scheduler::put(PCB::running);
		PCB::running=Scheduler::get();
		PCB::running->flags=PCB_RUNNING;

		cout<<"NEW RUNNING ID: "<<PCB::running->id<<'\n';

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		timer_count = PCB::running->quant;

		// restore sp
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}
	//Poziv stare prekidne rutine koju smo premestili na 60h
	if(!(timer_flags & TIMER_CHANGE_CONTEXT))
		asm int 60h;

	//Vise ne menjamo konteks, ali sacuvaj ostale flegove mozda su relevantne
	timer_flags = timer_flags & (~TIMER_CHANGE_CONTEXT);
}

