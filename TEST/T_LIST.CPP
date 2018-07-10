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

void print(List<double>& lst){
	for(List<double>::iter itr=lst.begin();itr!=lst.end();++itr)
		cout<<*itr<<" ";
	cout<<endl;
}

void TList::run(){
	List<double> lst;

	for(int i=0;i<N;i++)
		lst.add(i);
	print(lst);
	for(List<double>::iter itr = lst.begin(); itr!=lst.end();++itr){
		switch(itr.position()){
		case 2:
			cout<<"!2:"<<*itr<<'!'<<endl;
			break;
		case 5:
			break;
		case 4:
			lst.append(itr,4.5);
			break;
		case 8:
			lst.prepend(itr,6.5);
			break;
		case 11:
			lst.append(itr,9.5);
			break;
		default:
			break;
		}
	}
	print(lst);
	cout<<endl;
	for(int j=0;j<N;j++){
		cout << lst.get() << " ";
	}
	cout<<endl<<"LIST COUNT: "<<lst.count()<<endl;
}

#endif /* TEST_T_LIST_CPP_ */
