/*
 * T_SLIST.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */


#include "T_SLIST.H"
#include "PCB.H"
#include "SLIST.H"
#include <IOSTREAM.H>
#include <STDLIB.H>

void TSleepList::run(){
	PCB arr[10];
	SleepList sl;
	for(int j=0;j<100;j++)
		rand();
	for(int i=0;i<10;i++){
		arr[i].toSleep=rand()%10;
		cout<<arr[i].toSleep<<' ';
	}
	cout<<'\n';
	for(int k=0;k<10;k++){
		sl.add(&arr[k]);
		sl.print();
	}

	PCB*pcb;
	while(!sl.empty()){
		cout<<"update\n";
		sl.update();
		sl.print();
		while(pcb=sl.getReady()){
			cout<<"get: "<<pcb->id<<'\n';
		}
	}
}

