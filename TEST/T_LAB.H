/*
 * T_LAB.H
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#ifndef TEST_T_LAB_H_
#define TEST_T_LAB_H_

#include "TEST.H"
#include "THREAD.H"

class TLab:public Test{
	class A: public Thread{
		static unsigned lastId;
		unsigned id;
		void run();
	public:
		A();
	};
	void run();
};

#endif /* TEST_T_LAB_H_ */
