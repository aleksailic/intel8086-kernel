/*
 * KTHREAD.H
 *
 *  Created on: Jun 11, 2018
 *      Author: OS1
 */

#ifndef H_KTHREAD_H_
#define H_KTHREAD_H_

#include "SYS.H"
#include "IVTENTRY.H"
#include "VECTOR.H"
#include "STACK.H"

typedef void(*fn)(SysData*);

#define CHK_KTASK(t) KernelThread::task==t
#define SET_KTASK(t) KernelThread::task=t
#define CLR_KTASK() KernelThread::task=0

#define CLR_KFLAG(f) CLR_FLAG(KernelThread::captured,f)
#define CHK_KFLAG(f) CHK_FLAG(KernelThread::captured,f)
#define SET_KFLAG(f) SET_FLAG(KernelThread::captured,f)

#define STORE_REGS(pcb)\
	asm mov tsp, sp;\
	asm mov tss, ss;\
	asm mov tbp, bp;\
	(pcb)->sp = tsp;\
	(pcb)->ss = tss;\
	(pcb)->bp = tbp
#define RESTORE_REGS(pcb)\
	tsp = (pcb)->sp;\
	tss = (pcb)->ss;\
	tbp = (pcb)->bp;\
	asm mov sp, tsp;\
	asm mov ss, tss;\
	asm mov bp, tbp

enum{
	SET_KERNEL=0x1,
	FINISH_KERNEL=0x2
};

struct NestCall{
	unsigned sp;
	unsigned ss;
	unsigned bp;
};

class PCB;
class KernelSem;
class KernelEv;
class KernelThread{
	Vector<PCB*> pcbs;
	Vector<KernelSem*> semaphores;
	Vector<KernelEv*> events;
	Stack<NestCall> nestcalls;
	PCB* myPCB;

	static fn syscallfn[15];
	static void _sleep(SysData* data), _dispatch(SysData*data),
				_thread_create(SysData* data), _thread_start(SysData* data), _thread_join(SysData* data), _thread_destroy(SysData* data),
				_sem_create(SysData* data), _sem_wait(SysData* data), _sem_signal(SysData* data), _sem_val(SysData* data), _sem_destroy(SysData* data),
				_event_create(SysData* data),_event_wait(SysData* data), _event_signal(SysData* data), _event_destroy(SysData* data);

	static KernelThread* self;
	KernelThread();

	void run();
	static void interrupt routine(...);
public:
	static PCB* captured;
	static unsigned task;
	static const IVTNo ivtNo;
	static KernelThread* getKernelThread();
	static void triggerRun();
};

extern KernelThread* runningKernelThread;

#endif /* H_KTHREAD_H_ */
