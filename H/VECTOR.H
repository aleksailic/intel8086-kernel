/*
 * VECTOR.H
 *
 *  Created on: Jun 13, 2018
 *      Author: OS1
 */

#ifndef H_VECTOR_H_
#define H_VECTOR_H_

template <class T>
class Vector{
	T* data;
	unsigned cap;
	unsigned size;

	void resize(){
		T* n_data = new T[cap*2];
		for(int i=0;i<cap;i++)
			n_data[i]=data[i];
		delete[] data;
		data=n_data;
		cap*=2;
	}
public:
	Vector(unsigned cap):cap(cap),size(0){
		data = new T[cap];
	}
	void push_back(T& el){
		if(size==cap)
			resize();
		data[size++]=el;
	}
	T& operator[](int index){
		return data[index];
	}
};



#endif /* H_VECTOR_H_ */
