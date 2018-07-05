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

	THREAD_CREATE,
	THREAD_START,
	THREAD_JOIN,
	THREAD_DESTROY,

	SEM_CREATE,
	SEM_WAIT,
	SEM_SIGNAL,
	SEM_VAL,
	SEM_DESTROY,

	EVENT_CREATE,
	EVENT_WAIT,
	EVENT_SIGNAL,
	EVENT_DESTROY
};

struct SysData{
	SYSCALL id;
	void* params[4];
};

typedef int thread_t;
typedef unsigned sem_t;
typedef unsigned event_t;
typedef unsigned char IVTNo;
class Semaphore;
class Event;

void sys_sleep(Time time);
void sys_dispatch();
void sys_thread_create(StackSize sz, Time quant, Thread* thread,ID* threadId);
void sys_thread_start(ID threadId);
void sys_thread_join(ID threadId);
void sys_thread_destroy(ID threadId);

void sys_sem_create(sem_t* id,int value);
void sys_sem_wait(sem_t id,int toBlock,int* retCode);
void sys_sem_signal(sem_t id);
void sys_sem_val(sem_t id, int* val);
void sys_sem_destroy(sem_t id);

void sys_event_create(event_t*id,IVTNo ivtNo);
void sys_event_wait(event_t id);
void sys_event_signal(event_t id);
void sys_event_destroy(event_t id);

class Lock{
	static unsigned char val;
	Lock();
public:
	static void set();
	static void unset();
	static unsigned char isSet(); //ne postoji bool
};

#endif /* H_SYS_H_ */
