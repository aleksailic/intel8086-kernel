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

#define SEND(sysdata)\
	tcx=FP_SEG(&sysdata);\
	tdx=FP_OFF(&sysdata);\
	asm push cx;\
	asm push dx;\
	asm mov cx, tcx;\
	asm mov dx, tdx;\
	SET_KTASK(SET_KERNEL);\
	asm int 60h;\
	asm pop dx;\
	asm pop cx

#define PACK0(type)\
	asm cli;\
	SysData data;\
	data.id=type;\
	SEND(data);\
	asm sti

#define PACK1(type,arg1)\
	asm cli;\
	SysData data;\
	data.id=type;\
	data.params[0]=&arg1;\
	SEND(data);\
	asm sti

#define PACK2(type,arg1,arg2)\
	asm cli;\
	SysData data;\
	data.id=type;\
	data.params[0]=&arg1;\
	data.params[1]=&arg2;\
	SEND(data);\
	asm sti

#define PACK3(type,arg1,arg2,arg3)\
	asm cli;\
	SysData data;\
	data.id=type;\
	data.params[0]=&arg1;\
	data.params[1]=&arg2;\
	data.params[2]=&arg3;\
	SEND(data);\
	asm sti

#define PACK4(type,arg1,arg2,arg3,arg4)\
	asm cli;\
	SysData data;\
	data.id=type;\
	data.params[0]=&arg1;\
	data.params[1]=&arg2;\
	data.params[2]=&arg3;\
	data.params[3]=&arg4;\
	SEND(data);\
	asm sti


void sys_sleep(Time time){
	PACK1(SLEEP,time);
}
void sys_dispatch(){
	PACK0(DISPATCH);
}
void sys_thread_create(StackSize sz, Time quant, Thread* thread, ID* threadId){
	PACK4(THREAD_CREATE,sz,quant,thread,threadId);
}
void sys_thread_start(ID threadId){
	PACK1(THREAD_START,threadId);
}
void sys_thread_join(ID threadId){
	PACK1(THREAD_JOIN,threadId);
}

void sys_sem_create(sem_t* id,int value){
	PACK2(SEM_CREATE,id,value);
}
void sys_sem_wait(sem_t id,int toBlock,int* retCode){
	PACK3(SEM_WAIT,id,toBlock,retCode);
}
void sys_sem_signal(sem_t id){
	PACK1(SEM_SIGNAL,id);
}
void sys_sem_val(sem_t id, int* val){
	PACK2(SEM_VAL,id,val);
}
void sys_sem_destroy(sem_t id){
	PACK1(SEM_DESTROY,id);
}


void sys_event_wait(Event* evt){
	PACK1(EVENT_WAIT,evt);
}
void sys_event_signal(Event* evt){
	PACK1(EVENT_SIGNAL,evt);
}
