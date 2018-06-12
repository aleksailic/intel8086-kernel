/*
 * SYS.CPP
 *
 *  Created on: Jun 11, 2018
 *      Author: OS1
 */

#include "SYS.H"
#include "TIMER.H"
#include "KTHREAD.H"
#include <DOS.H>

static unsigned tcx;
static unsigned tdx;

#define PACK(sysdata,argNum,arg)\
	sysdata.params[argNum]=&arg;\
	tcx=FP_SEG(&sysdata);\
	tdx=FP_OFF(&sysdata);\
	asm push cx;\
	asm push dx;\
	asm mov cx, tcx;\
	asm mov dx, tdx;\
	SET_KTASK(SET_KERNEL);\
	asm int 60h;\
	asm pop dx;\
	asm pop cx;

#define PACK1(type,arg)\
	asm cli;\
	SysData data;\
	data.id=type;\
	PACK(data,0,arg)\
	asm sti;

#define PACK2(type,arg1,arg2)\
	asm cli;\
	SysData data;\
	data.id=type;\
	PACK(data,0,arg1)\
	PACK(data,1,arg2)\
	asm sti;

#define PACK3(type,arg1,arg2,arg3)\
	asm cli;\
	SysData data;\
	data.id=type;\
	PACK(data,0,arg1)\
	PACK(data,1,arg2)\
	PACK(data,2,arg3)\
	asm sti;

void sys_sleep(Time time){
	PACK1(SLEEP,time)
}
void sys_thread_create(StackSize sz, Time quant, ID* threadId){
	PACK3(THREAD_CREATE,sz,quant,threadId)
}
void sys_thread_start(ID threadId){
	PACK1(THREAD_START,threadId)
}
void sys_thread_join(ID threadId){
	PACK1(THREAD_JOIN,threadId)
}
void sys_sem_wait(Semaphore* sem){
	PACK1(SEM_WAIT,sem)
}
void sys_sem_signal(Semaphore* sem){
	PACK1(SEM_SIGNAL,sem)
}
void sys_event_wait(Event* evt){
	PACK1(EVENT_WAIT,evt)
}
void sys_event_signal(Event* evt){
	PACK1(EVENT_SIGNAL,evt)
}
