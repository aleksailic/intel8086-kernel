/*
 * FLIST.H
 * Implementation of FIFO queue using linked lists
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef H_FLIST_H_
#define H_FLIST_H_


template <class T>
class FList{
	struct T_w{
		T*el;
		T_w*next;
		T_w(T* el):el(el),next(0){}
	};
	T_w*first;
	T_w*last;
public:
	FList():first(0),last(0){}
	void add(T* el){
		last = (first ? last->next : first) = new T_w(el);
	}
	inline int empty(){
		return (first==0 ? 1 : 0);
	}
	T* get(){ //Vraca 0 ako je prazno
		if(empty())
			return 0;
		T*toReturn = first->el;
		T_w*toDelete = first;
		first=first->next;
		delete toDelete;
		return toReturn;
	}
	/*//Sluzi za test
	T* view(int index){
		T_w* itr=first;
		for(int i=0; i++<index && itr!=0;itr=itr->next);
		if(itr!=0)
			return itr->el;
		return 0;
	}*/
};

#endif /* H_FLIST_H_ */
