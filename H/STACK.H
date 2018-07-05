/*
 * STACK.H
 *
 *  Created on: Jun 14, 2018
 *      Author: OS1
 */

#ifndef H_STACK_H_
#define H_STACK_H_

template<class T>
class Stack{
	unsigned count;
	T*data;
public:
	Stack(int cap):count(0){
		data=new T[cap];
	}
	void push(const T& el){
		data[count++]=el;
	}
	T& peek(){
		return data[count-1];
	}
	T& pop(){
		return data[--count];
	}
	int empty(){
		return (count==0 ? 1: 0);
	}
};





#endif /* H_STACK_H_ */
