/*
 * FLIST.H
 * Implementation of FIFO queue using linked lists
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef H_FLIST_H_
#define H_FLIST_H_

class PCB;
class FList{
	struct PCB_w{
		PCB*pcb;
		PCB_w*next;
		PCB_w(PCB* pcb):pcb(pcb),next(0){}
	};
	PCB_w*first;
	PCB_w*last;
public:
	FList();
	void add(PCB* pcb);
	void print(); //for debugging purposes
	PCB* get(); //Vraca 0 ako je prazno
	inline int empty();
};



#endif /* H_FLIST_H_ */
