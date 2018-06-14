/*
 * EVENT.H
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef H_EVENT_H_
#define H_EVENT_H_

typedef unsigned char IVTNo;
typedef unsigned event_t;
class KernelEv;

class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();
protected:
	friend class KernelEv;
	void signal();
private:
	event_t id;
};

#endif /* H_EVENT_H_ */
