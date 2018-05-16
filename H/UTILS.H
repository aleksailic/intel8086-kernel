/*
 * UTILS.H
 *
 *  Created on: May 10, 2018
 *      Author: OS1
 */

#ifndef H_UTILS_H_
#define H_UTILS_H_

#define PREPAREENTRY(numEntry,callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry newEntry##numEntry(numEntry,inter##numEntry);\
	void interrupt inter##numEntry(...){\
		newEntry##numEntry.signal();\
		if(callOld==1)\
			newEntry##numEntry.callOld();\
	}

void interrupt timer();
void changeTimerRoutine();
void restoreTimerRoutine();


#endif /* H_UTILS_H_ */
