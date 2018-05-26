#include <IOSTREAM.H>
#include <DOS.H>

#include "UTILS.H"

#include "TEST.H"
#include "T_SEMPC.H"
#include "T_FLIST.H"
#include "T_LAB.H"
#include "T_SLIST.H"

#define lock asm cli
#define unlock asm sti

int userMain(int argc, char*argv[]){
	Test * test = new TLab();
	test->run();
	delete test;
	return 0;
}

int main(int argc,char*argv[]){
	changeTimerRoutine();
	userMain(argc,argv);
	restoreTimerRoutine();
	cout<<"HAPPY END!";
	return 0;
}
