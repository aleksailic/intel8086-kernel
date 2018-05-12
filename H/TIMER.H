/*
 * TIMER.H
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "PCB.H"
#include "SCHEDULE.H"

#define CHK_TFLAG(f) (tflags & f)
#define CLR_TFLAG(f) tflags &= ~f
#define SET_TFLAG(f) tflags |= f

enum{
	TIMER_CHANGE_CONTEXT = 0x1
};
extern volatile int tflags;
extern volatile int tcount;

//Specijalizovana lista za cekanje
class SleepList{
	struct PCB_w{
		PCB*pcb;
		PCB_w*next;
		PCB_w(PCB* pcb){
			this->pcb=pcb;
			this->next=0;
		}
	};
	PCB_w* head;
public:
	SleepList();
	void add(PCB* pcb);
	void update();
	PCB* getReady(); //Vraca NULL ako nije gotov
};

void interrupt timer();

extern SleepList sl;

#endif /* TIMER_H_ */
