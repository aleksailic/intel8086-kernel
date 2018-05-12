/*
 * TIMER.CPP
 *
 *  Created on: May 12, 2018
 *      Author: OS1
 */

#include <IOSTREAM.H>
#include "TIMER.H"

volatile int tflags=0;
volatile int tcount=2;

static unsigned tsp;
static unsigned tss;
static unsigned tbp;
static PCB* wakeMe=0;

SleepList sl;

void interrupt timer(){	// prekidna rutina
	if (!CHK_TFLAG(TIMER_CHANGE_CONTEXT) && !CHK_RFLAG(PCB_UNLIMITED))
		tcount--;
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

		if(!CHK_TFLAG(TIMER_CHANGE_CONTEXT))
			sl.update();
		if(CHK_RFLAG(PCB_SLEEPING))
			sl.add(PCB::running);

		while(wakeMe=sl.getReady()){
			CLR_FLAG(wakeMe->flags,PCB_SLEEPING);
			Scheduler::put(wakeMe);
		}

		if(!CHK_RFLAG(PCB_FINISHED) && !CHK_RFLAG(PCB_SLEEPING))
			Scheduler::put((PCB*)PCB::running);
		PCB::running=Scheduler::get();

		SET_STATUS_RFLAG(PCB_RUNNING); //brise ostale statusne stavlja onaj setovan
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


SleepList::SleepList(){
	head=0;
}
void SleepList::add(PCB* pcb){
	PCB_w*el=new PCB_w(pcb);
	PCB_w**itr=&head;
	for (; *itr != 0; itr = &((*itr)->next)) {
		if ((*itr)->pcb->toSleep >= el->pcb->toSleep) { //ako je vece situacija recimo 3-> [7 5 0 0]
			(*itr)->pcb->toSleep -= el->pcb->toSleep;
			break;
		}else {//situacija manje 7-> [3 2 1 0 0]
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

