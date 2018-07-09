/*
 * LIST.H
 *
 *  Created on: Jul 9, 2018
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_

template <class T>
class List{
protected:
  struct T_w{
    T el; //pravi kopiju!
    T_w*next;

    T_w(const T& el,T_w*next=0):el(el),next(next){}
  };
  T_w*first;
  T_w*last;
  unsigned mcount;
public:
  class iter{
    T_w** data;
    unsigned mposition;
  public:
    iter(T_w** data):data(data),mposition(0){}
    iter& next(){
      data=&((*data)->next);
      mposition++;
      return *this;
    }
    T& operator*() { return (*data)->el; }

    int operator==(const iter& rhs){
      return (*data==*rhs.data ? 1 : 0);
    }
    int operator!=(const iter& rhs){
      return (*data==*rhs.data ? 0 : 1);
    }
    iter& operator++(){
      return next();
    }
    void remove(){
      T_w* next = (*data)->next;
      delete (*data);
      *data=next;
    }
    unsigned position()const{
        return mposition;
    }
  };
  List():first(0),last(0),mcount(0){}
  virtual ~List(){
	  /*T_w*itr=first;
	  while(itr!=0){
		  T_w* toDelete = itr;
		  itr=itr->next;
		  delete toDelete;
	  }*/
  }

  iter begin(){ return iter(&first); }
  iter end() { return (last==0 ? iter(&(last)) : iter(&(last->next))); }

  virtual List& add(const T& el){
    last = (first ? last->next : first) = new T_w(el);
    mcount++;
    return *this;
  }
  int empty()const{
    return (first==0 ? 1 : 0);
  }
  virtual T get(){
	  if(empty())
		  return 0;
	  T toReturn = first->el;
	  T_w*toDelete = first;
	  first=first->next;
	  delete toDelete;
	  mcount--;
	  return toReturn;
  }
  unsigned count()const{return mcount;}
};

#endif /* H_LIST_H_ */
