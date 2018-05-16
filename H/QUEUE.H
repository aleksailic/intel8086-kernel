/*
 * QUEUE.H
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#ifndef H_QUEUE_H_
#define H_QUEUE_H_


class PCB;
class Queue{
	PCB** data; //to je u startu alokacija od 20B
	unsigned front,rear,cap;
public:
	Queue(unsigned capacity);
	inline int empty();
	inline int full();
	void put(PCB* pcb);
	PCB* get();
};



#endif /* H_QUEUE_H_ */
