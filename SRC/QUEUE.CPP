/*
 * QUEUE.CPP
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#include "QUEUE.H"

Queue::Queue(unsigned capacity):cap(capacity),front(0),rear(0){
	data=new PCB*[capacity];
}
void Queue::put(PCB* pcb){
	rear=(rear+1) % cap;
	if(!full()){
		data[rear]=pcb;
	}
}
PCB* Queue::get(){
	if(!empty()){
		front=(front+1) % cap;
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


