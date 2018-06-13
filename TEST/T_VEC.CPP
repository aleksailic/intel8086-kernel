/*
 * T_VEC.CPP
 *
 *  Created on: Jun 13, 2018
 *      Author: OS1
 */

#include "T_VEC.H"
#include "VECTOR.H"
#include <IOSTREAM.H>
#include <STDLIB.H>

static const int N = 50;
static const int SEED = 1005;
static const int CAP = 10;

static void setSeed(int seed){
	for(int i=0;i<seed;i++)
		rand();
}

void TVec::run(){
	int arr[N];
	Vector<int> vec(CAP);

	for(int i=0; i<N; i++)
		vec.push_back(arr[i]);
	for(int j=0;j<N;j++)
		cout<<"vec["<<j<<"]:"<<vec[j]<<", "<< (vec[j]==arr[j] ? "passed" : "failed") <<'\n';
}


