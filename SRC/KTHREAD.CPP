/*
 * KTHREAD.CPP
 *
 *  Created on: Jun 11, 2018
 *      Author: OS1
 */


#include "KTHREAD.H"
#include "PCB.H"
#include "TIMER.H"
#include "SYS.H"
#include <DOS.H>

#define GETPARAM(type,id) *((type*)sysdata->params[id])

PCB* KernelThread::myPCB=new PCB(defaultStackSize,0,0,PCB_KERNEL,0); //id: 0

static void _sleep(){}
static void _thread_create(){}
static void _thread_join(){}
static void _thread_start(){}
static void _sem_wait(){}
static void _sem_signal(){}
static void _event_wait(){}
static void _event_signal(){}

SysData* KernelThread::sysdata=0;
fn KernelThread::syscallfn[10]={0};

void KernelThread::init(){
	syscallfn[SLEEP]=_sleep;
	syscallfn[THREAD_CREATE]=_thread_create;
	syscallfn[THREAD_JOIN]=_thread_join;
	syscallfn[THREAD_START]=_thread_start;
	syscallfn[SEM_WAIT]=_sem_wait;
	syscallfn[SEM_SIGNAL]=_sem_signal;
	syscallfn[EVENT_SIGNAL]=_event_signal;
	syscallfn[EVENT_WAIT]=_event_wait;
}
void KernelThread::run(){
	//dozvoljavamo prekide, ali ne damo kontekst!
	Lock::set();
	asm sti;
	unsigned tcx;
	unsigned tdx;
	asm{
		mov tcx,cx
		mov tdx,dx
	}
	sysdata=(SysData*)MK_FP(tcx,tdx); //get stored data
	syscallfn[sysdata->id](); //call the function

	SET_TFLAG(Timer::SET_SAVED); //restore PCB of the one that requested syscall
	Lock::unset();
}
