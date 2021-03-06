#include <IOSTREAM.H>
#include "TIMER.H"

#include "TEST.H"
#include "T_SEMPC.H"
#include "T_FLIST.H"
#include "T_LLIST.H"
#include "T_LAB.H"
#include "T_SLIST.H"
#include "T_VEC.H"
#include "T_LIST.H"

//extern int userMain(int argc,char*argv[]);
int userMain(int argc, char*argv[]){
	Test * test = new TLab();
	test->run();
	delete test;
	return 0;
}

int main(int argc,char*argv[]){
	Timer::create();
	userMain(argc,argv);
	Timer::destroy();
	cout<<"HAPPY END!";
	return 0;
}
