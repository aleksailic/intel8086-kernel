/*
 * SEMAPHOR.H
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */

#ifndef H_SEMAPHOR_H_
#define H_SEMAPHOR_H_


class KernelSem;

class Semaphore{
public:
	Semaphore (int init=1);
	virtual ~Semaphore();

	virtual int wait(int toBlock);
	virtual void signal();

	int val() const;
private:
	KernelSem* myImpl;
};


#endif /* H_SEMAPHOR_H_ */
