/*
 * MIDWAY.H
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef TEST_T_SEMPC_H_
#define TEST_T_SEMPC_H_

#include "TEST.H"
#include "THREAD.H"
#include "SEMAPHOR.H"

class TSemProdCons:public Test{
	class BoundedBuffer{
		Semaphore mutex;
		Semaphore itemAvail;
		Semaphore spaceAvail;
		int data[10];
		int front,rear;
	public:
		BoundedBuffer():mutex(1),itemAvail(0),spaceAvail(10),front(0),rear(0){}
		void put(int elem);
		int get();
	};
	class Producer:public Thread{
		BoundedBuffer*buf;
		void run();
		int elem;
	public:
		Producer(BoundedBuffer*buffer,int element=2):buf(buffer),elem(element){}
	};
	class Consumer:public Thread{
		BoundedBuffer*buf;
		void run();
		int elem;
	public:
		Consumer(BoundedBuffer*buffer):buf(buffer),elem(0){}
	};
	void run();
};


#endif /* TEST_T_SEMPC_H_ */
