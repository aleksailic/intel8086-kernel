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

void interrupt timer();

#endif /* TIMER_H_ */
