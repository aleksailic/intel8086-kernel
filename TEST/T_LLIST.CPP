/*
 * T_LLIST.CPP
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#include "T_LLIST.H"
#include "LLIST.H"
#include <IOSTREAM.H>
#include <STDLIB.H>

static void setSeed(int seed){
	for(int i=0;i<seed;i++)
		rand();
}
static const int N = 50;
static const int SEED = 1005;

void TLList::run(){
	int arr[N];
	LList<int> ll;
	setSeed(SEED);
	int index;
	for(int i=0;i<N;i++){
		if(rand()%2){
			index=rand() % N;
			cout<<"stavljam: " << arr[index] <<'\n';
			ll.add(&arr[index]);
		}else{
			if(!ll.empty())
				cout<<"uzimam: "<< *(ll.get())<<'\n';
			else
				cout<<"prazna!\n";
		}
		/*for(int j=0;;j++){
			int* el = ll.view(j);
			if(el==0)
				break;
			cout<< *el <<"|";
		}*/
	}
}


