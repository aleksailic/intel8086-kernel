/*
 * SEM.H
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#ifndef H_KSEM_H_
#define H_KSEM_H_

#include "QUEUE.H"

class KernelSem{
	int val;
	int mutex;
	Queue blocked;
	inline void block();
	inline void unblock();
	inline void lock();
	inline void unlock();
public:
	KernelSem(int initValue=0);
	void signal();
	int wait(int toBlock=1);
	inline int getVal()const;
	int setVal(int value); //za neke debugging potrebe, generalno no-no
};


#endif /* H_KSEM_H_ */
