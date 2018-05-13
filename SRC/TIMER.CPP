/*
 * TIMER.CPP
 *
 *  Created on: May 12, 2018
 *      Author: OS1
 */

#include <IOSTREAM.H>
#include "TIMER.H"
#include "SLIST.H"

volatile int tflags=0;
volatile int tcount=2;

static unsigned tsp;
static unsigned tss;
static unsigned tbp;
static PCB* wakeMe=0;

static SleepList sl;

void interrupt timer(){	// prekidna rutina
	if (!tflags && !CHK_RFLAG(PCB_UNLIMITED)) //za bilo koji fleg
		tcount--;
	//promena konteksta
	if ( (tcount == 0 && !CHK_RFLAG(PCB_UNLIMITED)) || CHK_TFLAG(TIMER_CHANGE_CONTEXT)) {
		//cuvamo sp
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;
		CLR_RFLAG(PCB_RUNNING);
		SET_RFLAG(PCB_BLOCKED);

		//cout<<"OLD ID: "<<PCB::running->id<<" FLAGS: "<<PCB::running->flags<<'\n';
		//asm cli;

		if(!CHK_TFLAG(TIMER_CHANGE_CONTEXT)){
			sl.update();
			while(wakeMe=sl.getReady()){
				CLR_FLAG(wakeMe->flags,PCB_SLEEPING);
				Scheduler::put(wakeMe);
			}
		}
		if(CHK_RFLAG(PCB_SLEEPING))
			sl.add(PCB::running);

		if(!CHK_RFLAG(PCB_FINISHED) && !CHK_RFLAG(PCB_SLEEPING))
			Scheduler::put((PCB*)PCB::running);
		PCB::running=Scheduler::get();

		SET_STATUS_RFLAG(PCB_RUNNING); //brise ostale statusne stavlja samo running
		//cout<<"NEW ID: "<<PCB::running->id<<" FLAGS: "<<PCB::running->flags<<'\n';
		//asm cli;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		tcount = PCB::running->quant;

		// restore sp
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}
	//Poziv stare prekidne rutine koju smo premestili na 60h
	if(!CHK_TFLAG(TIMER_CHANGE_CONTEXT))
		asm int 60h;

	//Vise ne menjamo konteks, ali sacuvaj ostale flegove mozda su relevantne
	CLR_TFLAG(TIMER_CHANGE_CONTEXT);
}

