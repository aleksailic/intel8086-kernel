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

typedef void(*fn)(SysData*);

#define CHK_KTASK(t) KernelThread::task==t
#define SET_KTASK(t) KernelThread::task=t
#define CLR_KTASK() KernelThread::task=0

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
class KernelThread{
	PCB* myPCB;
	fn syscallfn[10];
	LList<NestCall> nestcalls;
	void run();
	KernelThread();

	static KernelThread* self;
	static void interrupt routine(...);
public:
	static PCB* blocked;
	static unsigned task;
	static const IVTNo ivtNo;
	static KernelThread* getKernelThread();
	static void triggerRun();
};

extern KernelThread* runningKernelThread;

#endif /* H_KTHREAD_H_ */
