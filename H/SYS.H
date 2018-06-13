/*
 * SYS.H
 *
 *  Created on: Jun 11, 2018
 *      Author: OS1
 */

#ifndef H_SYS_H_
#define H_SYS_H_

#include "THREAD.H"

enum SYSCALL{
	SLEEP,
	DISPATCH,

	THREAD_START,
	THREAD_CREATE,
	THREAD_JOIN,

	SEM_WAIT,
	SEM_SIGNAL,

	EVENT_SIGNAL,
	EVENT_WAIT
};

struct SysData{
	SYSCALL id;
	void* params[4];
};

typedef unsigned thread_t;
class Semaphore;
class Event;

void sys_sleep(Time time);
void sys_dispatch();

void sys_thread_create(StackSize sz, Time quant, Thread* thread,ID* threadId);
void sys_thread_start(ID threadId);
void sys_thread_join(ID threadId);

void sys_sem_wait(Semaphore* sem);
void sys_sem_signal(Semaphore* sem);

void sys_event_wait(Event* evt);
void sys_event_signal(Event* evt);

#endif /* H_SYS_H_ */
