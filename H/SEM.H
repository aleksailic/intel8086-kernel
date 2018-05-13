/*
 * SEM.H
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#ifndef H_SEM_H_
#define H_SEM_H_

#include "QUEUE.H"

class Semaphore{
	int val;
	int mutex;
	Queue blocked;
	inline void block();
	inline void unblock();
	inline void lock();
	inline void unlock();
public:
	Semaphore(int initValue=0);
	void signal();
	void wait();
};


#endif /* H_SEM_H_ */
