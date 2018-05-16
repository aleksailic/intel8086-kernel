/*
 * KEVENT.H
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef H_KEVENT_H_
#define H_KEVENT_H_

class PCB;
class KernelEv{
	PCB* myPCB;
	int val;
	int mutex;

	inline void lock();
	inline void unlock();
	inline void block();
	inline void unblock();
public:
	KernelEv();
	void wait();
	void signal();
};



#endif /* H_KEVENT_H_ */
