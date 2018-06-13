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

static const int N = 21;
static const int SEED = 1005;
static const int CAP = 10;
static const int RM_N = 2;

static void setSeed(int seed){
	for(int i=0;i<seed;i++)
		rand();
}

void TVec::run(){
	int arr[N];
	int i;
	Vector<int> vec(CAP);
	setSeed(SEED);

	for(i=0;i<N;i++)
		arr[i]=rand();

	for(i=0; i<N; i++)
		vec.push_back(arr[i]);
	for(int j=0;j<N;j++)
		cout<<"vec["<<j<<"]:"<<vec[j]<<", "<< (vec[j]==arr[j] ? "passed" : "failed") <<'\n';
	for(i=0;i<RM_N; i++){
		int ind=rand()%N;
		cout<<"oslobodio ind "<<ind<<'\n';
		vec.remove(ind);
	}
	for(i=0;i<RM_N;i++){
		int val=rand();
		int ind=vec.add(val);
		cout<<"dodao " <<val<<" na ind: "<<ind<<", "<<(vec[ind]==val ? "passed" : "failed")<<'\n';
	}

	cout<<"vec count: "<<vec.count()<<'\n';
	cout<<"vec size: "<<vec.size()<<'\n';
	cout<<"vec capacity: "<<vec.capacity()<<'\n';
}


