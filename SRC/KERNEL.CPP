#include <IOSTREAM.H>
#include <DOS.H>

#include "TIMER.H"
#include "UTILS.H"

#define lock asm cli
#define unlock asm sti

int userMain(int argc, char*argv[]){
	cout<<"CAO";
}

int main(int argc,char*argv[]){
  changeTimerRoutine();
  userMain(argc,argv);
  restoreTimerRoutine();
  return 0;
}
