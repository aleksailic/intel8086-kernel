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

typedef void(*fn)();

class PCB;
class KernelThread{
	static PCB* myPCB;

	static SysData* sysdata;
	static fn syscallfn[10];

	static void run();
public:
	static void init();
};




#endif /* H_KTHREAD_H_ */
