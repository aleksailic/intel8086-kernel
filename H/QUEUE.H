/*
 * QUEUE.H
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#ifndef H_QUEUE_H_
#define H_QUEUE_H_

const int MAX_QUEUE = 10;

class PCB;
class Queue{
	PCB* data[MAX_QUEUE]; //to je u startu alokacija od 20B
	unsigned front;
	unsigned rear;
public:
	Queue();
	inline int empty();
	inline int full();
	void put(PCB* pcb);
	PCB* get();
};



#endif /* H_QUEUE_H_ */
