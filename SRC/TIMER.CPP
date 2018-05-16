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
static PCB* nop=PCB::createNOP();

static SleepList sl;
volatile timestamp sysruntime=0;


void interrupt timer(){	// prekidna rutina
	if(!tflags){ //poziv preko prekida
		tcount--;
		sysruntime++;
		//tick();
		sl.update();
		while(wakeMe=sl.getReady()){
			CLR_FLAG(wakeMe->flags,PCB_SLEEPING);
			Scheduler::put(wakeMe);
		}
	}
	//promena konteksta
	if ( (tcount == 0 && !CHK_RFLAG(PCB_UNLIMITED)) || CHK_TFLAG(TIMER_CHANGE_CONTEXT)) {
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

		tcount = PCB::running->quant;

		// restore sp
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}
	//Poziv stare prekidne rutine koju smo premestili na 60h
	if(!CHK_TFLAG(TIMER_CHANGE_CONTEXT)) asm int 60h;
	//Vise ne menjamo konteks, ali sacuvaj ostale flegove mozda su relevantne
	CLR_TFLAG(TIMER_CHANGE_CONTEXT);
}

