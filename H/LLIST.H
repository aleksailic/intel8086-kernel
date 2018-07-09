/*
 * LLIST.H
 * Implementation of LIFO queue using linked lists
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef H_LLIST_H_
#define H_LLIST_H_

#include "LIST.H"

template <class T>
class LList: public List<T>{
public:
	LList(){}
	List<T>& add(const T& el){
	    first = new T_w(el,first);
	    mcount++;
	    return *this;
	}
};

#endif /* H_LLIST_H_ */
