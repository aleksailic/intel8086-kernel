/*
 * T_FLIST.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "T_FLIST.H"
#include "PCB.H"
#include <IOSTREAM.H>
#include <STDLIB.H>
#include "LIST.H"

static void setSeed(int seed){
	for(int i=0;i<seed;i++)
		rand();
}
static const int N = 50;
static const int SEED = 1005;

void TFList::run(){
	PCB arr[N];
	List<PCB*> fl;
	setSeed(SEED);
	int index;
	for(int i=0;i<N;i++){
		if(rand()%2){
			index=rand() % N;
			cout<<"stavljam: " << arr[index].id <<'\n';
			fl.add(&arr[index]);
		}else{
			if(!fl.empty())
				cout<<"uzimam: "<<fl.get()->id<<'\n';
			else
				cout<<"prazna!\n";
		}

		/*for(int j=0;;j++){
			PCB* el = fl.view(j);
			if(el==0)
				break;
			cout<< "pcb"<<el->id<<"|";
		}*/

	}
}


