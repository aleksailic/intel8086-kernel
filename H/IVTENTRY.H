/*
 * IVTENTRY.H
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class KernelEv;
class IVTEntry{
	IVTNo numEntry;
	pInterrupt oldRoutine;
	pInterrupt newRoutine;
	KernelEv* event;
public:
	IVTEntry(IVTNo numEntry,pInterrupt routine);
	~IVTEntry();

	void setEvent(KernelEv* event);
	void callOld(); //calls old routine
	void signal(); //signals the event!

	static IVTEntry* data[256];
	static IVTEntry* lastEntry;
	static IVTEntry* fetch(IVTNo id);
};

#define PREPAREENTRY(numEntry,oldCall) \
	void interrupt inter##numEntry(...){ \
		IVTEntry::data[##numEntry]->signal(); \
		if(oldCall==1)\
			IVTEntry::data[##numEntry]->callOld(); \
	} \
	IVTEntry* entry##numEntry=new IVTEntry(numEntry,inter##numEntry); \


#endif /* H_IVTENTRY_H_ */
