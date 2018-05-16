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

TLab::A::A():Thread(1024,10){id=lastId++;}
unsigned TLab::A::lastId=0;
void TLab::A::run(){
	for (int i =0; i <8; i++) {
		asm cli;
		cout<<"u a"<< id <<"() i = "<<i<<endl;
		asm sti;
		Thread::sleep(rand() % 10);
	}
}

void TLab::run(){
	asm cli;
	cout<<"Pokrece se main\n";
	asm sti;
	A thA[4];

	for(int j=0;j<4;j++)
		thA[j].start();

	for(int i=0;i<3;i++){
		asm cli;
		cout<<"main "<<i<<endl;
		asm sti;

		Thread::sleep(4);
	}
	for(int k=0;k<4;k++)
		thA[k].waitToComplete();
}


#endif /* TEST_T_LAB_CPP_ */
