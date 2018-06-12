/*
 * SEM.H
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#ifndef H_KSEM_H_
#define H_KSEM_H_

#include "FLIST.H"

class PCB;
class KernelSem{
	int val;
	int mutex;
	FList<PCB> blocked;
	inline void block();
	inline void unblock();
	inline void lock();
	inline void unlock();
public:
	KernelSem(int initValue=1);
	void signal();
	int wait(int toBlock=1);
	inline int getVal()const;
	void setVal(int value); //za neke debugging potrebe, generalno no-no
};


#endif /* H_KSEM_H_ */
