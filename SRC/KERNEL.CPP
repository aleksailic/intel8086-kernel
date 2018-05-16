#include <IOSTREAM.H>
#include <DOS.H>

#include "UTILS.H"

#include "TEST.H"
#include "T_SEMPC.H"
#include "T_FLIST.H"

#define lock asm cli
#define unlock asm sti

int userMain(int argc, char*argv[]){
	Test * test = new TFList();
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
