/*
 * THREAD.H
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; //time, x55ms
const Time defaultTimeSlice = 5; // default = 2*55ms
typedef int ID;

class PCB;
class Thread{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	Thread (StackSize stack=defaultStackSize, Time timeSlice = defaultTimeSlice);
	static void sleep(Time toSleep);
protected:
	friend class PCB;
	virtual void run(){}
private:
	PCB* myPCB;
};

void dispatch();



#endif /* THREAD_H_ */
