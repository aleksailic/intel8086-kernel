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

enum{
	TIMER_CHANGE_CONTEXT = 0x1
};
extern volatile int timer_flags;
extern volatile int timer_count;

void interrupt timer();

#endif /* TIMER_H_ */
