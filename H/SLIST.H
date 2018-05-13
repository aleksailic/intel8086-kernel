/*
 * SLIST.H
 *
 *  Created on: May 13, 2018
 *      Author: OS1
 */

#ifndef H_SLIST_H_
#define H_SLIST_H_

class PCB;
//Specijalizovana lista za cekanje
class SleepList{
	struct PCB_w{
		PCB*pcb;
		PCB_w*next;
		PCB_w(PCB* pcb){
			this->pcb=pcb;
			this->next=0;
		}
	};
	PCB_w* head;
public:
	SleepList();
	void add(PCB* pcb);
	void update();
	void print(); //for debugging purposes
	PCB* getReady(); //Vraca 0 ako nije gotov, obazrivo!
	int empty(){ return (head==0 ? 1 : 0); }
};

#endif /* H_SLIST_H_ */
