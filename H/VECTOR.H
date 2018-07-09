/*
 * VECTOR.H
 *
 *  Created on: Jun 13, 2018
 *      Author: OS1
 */

#ifndef H_VECTOR_H_
#define H_VECTOR_H_

#include "LIST.H"
#include "FLIST.H"

template <class T>
class Vector{
	T* data;
	unsigned mcap;
	unsigned msize;
	unsigned mcount;
	FList<int> free; //cuva ispraznjene indekse

	void resize(){
		T* n_data = new T[mcap*2];
		for(int i=0;i<mcap;i++)
			n_data[i]=data[i];
		delete[] data;
		data=n_data;
		mcap*=2;
	}
public:
	Vector(unsigned cap):mcap(cap),msize(0),mcount(0){
		data = new T[cap];
	}
	void push_back(const T& el){
		if(msize==mcap)
			resize();
		data[msize++]=el;
		mcount++;
	}
	int add(const T& el){ //pametna dodela
		if(free.empty()){
			push_back(el);
			return msize-1;
		}else{
			int ind = *free.get();
			data[ind]=el;
			mcount++;
			return ind;
		}
	}
	void remove(int index){
		free.add(new int(index));
		mcount--;
	}
	T& operator[](int index){
		return data[index];
	}

	unsigned count()const{
		return mcount;
	}
	unsigned size()const{
		return msize;
	}
	unsigned capacity()const{
		return mcap;
	}
};



#endif /* H_VECTOR_H_ */
