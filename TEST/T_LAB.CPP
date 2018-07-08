/*
 * T_LAB.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef TEST_T_LAB_CPP_
#define TEST_T_LAB_CPP_

#include "T_LAB.H"
#include <STDLIB.H>
#include <IOSTREAM.H>
#include "TIMER.H"

#define TH_COUNT 4
#define TH_NUM 5
#define MAIN_NUM 3

TLab::A::A():Thread(1024,10){id=lastId++;}
unsigned TLab::A::lastId=0;

void TLab::A::run(){
	for (int i =0; i <TH_NUM; i++) {
		asm cli;
		cout<<"u a"<< id <<"() i = "<<i<<endl;
		asm sti;

		//Thread::sleep(rand()%10);
		for(int j=0;j<30000;j++)
			for(int k=0;k<30000;k++);
	}
}

void TLab::run(){
	asm cli;
	cout<<"Pokrece se main\n";
	asm sti;
	A thA[TH_COUNT];

	for(int j=0;j<TH_COUNT;j++)
		thA[j].start();

	for(int i=0;i<MAIN_NUM;i++){
		asm cli;
		cout<<"main "<<i<<endl;
		asm sti;

		int rnd=rand();
		for(int j=0;j<30000;j++)
			for(int k=0;k<rnd;k++);
	}
	for(int k=0;k<TH_COUNT;k++){
		thA[k].waitToComplete();
		//cout<<k<<": complete!\n";
	}
}

#endif /* TEST_T_LAB_CPP_ */
