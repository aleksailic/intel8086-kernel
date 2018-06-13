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
#include "LLIST.H"
#include "VECTOR.H"

typedef void(*fn)(SysData*);

#define CHK_KTASK(t) KernelThread::task==t
#define SET_KTASK(t) KernelThread::task=t
#define CLR_KTASK() KernelThread::task=0

#define CLR_KFLAG(f) CLR_FLAG(KernelThread::blocked,f)
#define CHK_KFLAG(f) CHK_FLAG(KernelThread::blocked,f)
#define SET_KFLAG(f) SET_FLAG(KernelThread::blocked,f)

enum{
	SET_KERNEL=0x1,
	FINISH_KERNEL=0x2
};

struct NestCall{
	unsigned sp;
	unsigned ss;
	unsigned bp;
	NestCall(unsigned sp,unsigned ss,unsigned bp):sp(sp),ss(ss),bp(bp){}
};

class PCB;
class KernelSem;
class KernelEv;
class KernelThread{
	Vector<PCB*> pcbs;
	Vector<KernelSem*> semaphores;
	Vector<KernelEv*> events;

	PCB* myPCB;
	fn syscallfn[10];
	LList<NestCall> nestcalls;
	void run();
	KernelThread();

	static KernelThread* self;
	static void interrupt routine(...);

	static void _sleep(SysData* data);
	static void _dispatch(SysData* data);
	static void _thread_create(SysData* data);
	static void _thread_join(SysData* data);
	static void _thread_start(SysData* data);
	static void _sem_wait(SysData* data);
	static void _sem_signal(SysData* data);
	static void _event_wait(SysData* data);
	static void _event_signal(SysData* data);
public:
	static PCB* blocked;
	static unsigned task;
	static const IVTNo ivtNo;
	static KernelThread* getKernelThread();
	static void triggerRun();
};

extern KernelThread* runningKernelThread;

#endif /* H_KTHREAD_H_ */
