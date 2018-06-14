/*
 * TIMER.H
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef unsigned long timestamp;
typedef void interrupt (*pInterrupt)(...);
extern volatile timestamp sysruntime;

class SleepList;
class Timer{
	static pInterrupt oldRoutine;
	static const int routineNo;

	static inline void changeRoutine();
	static inline void restoreRoutine();
	static void interrupt routine(...);
	Timer();
public:
	static volatile int count;
	static SleepList* sleeplist;
	static void create();
	static void destroy();
};

#endif /* TIMER_H_ */
