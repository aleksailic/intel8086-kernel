/*
 * T_LIST.CPP
 *
 *  Created on: Jul 9, 2018
 *      Author: OS1
 */

#ifndef TEST_T_LIST_CPP_
#define TEST_T_LIST_CPP_

#include "T_LIST.H"
#include <IOSTREAM.H>
#include "LIST.H"
#include <STDLIB.H>

#define N 10

void TList::run(){
	List<int> lst;

	for(int i=0;i<N;i++)
		lst.add(rand());

	for(List<int>::iter itr = lst.begin(); itr!=lst.end();++itr){
		cout << *itr << " ";
		if(itr.position()==4){
			cout<<"\nSADA SAM NA 4tom elementu (" << *itr << ")\n";
		}
	}
	cout<<endl;
	for(int j=0;j<N;j++){
		cout << lst.get() << " ";
	}
}

#endif /* TEST_T_LIST_CPP_ */
