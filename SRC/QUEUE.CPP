/*
 * QUEUE.CPP
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#include "QUEUE.H"

Queue::Queue(){
	front=rear=0;
}
void Queue::put(PCB* pcb){
	rear=(rear+1) % MAX_QUEUE;
	if(!full()){
		data[rear]=pcb;
	}
}
PCB* Queue::get(){
	if(!empty()){
		front=(front+1) % MAX_QUEUE;
		return data[front];
	}
	return 0;
}
int Queue::empty(){
	return ( front==rear ? 1 : 0);
}
int Queue::full(){
	return ( front==rear ? 1 : 0);
}


