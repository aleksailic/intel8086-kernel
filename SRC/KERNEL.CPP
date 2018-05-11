#include <IOSTREAM.H>
#include <DOS.H>

#include "TIMER.H"
#include "UTILS.H"

#include "PCB.H"

#define lock asm cli
#define unlock asm sti

class A: public Thread{
	void run();
};
void A::run(){
	for (int i =0; i < 30; ++i) {
		lock;
		cout<<"u a() i = "<<i<<endl;
		unlock;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}
class B: public Thread{
	void run();
};
void B::run(){
	for (int i =0; i < 30; ++i) {
		lock;
		cout<<"u b() i = "<<i<<endl;
		unlock;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}


int userMain(int argc, char*argv[]){
	lock;
	cout<<"Pokrece se main";
	A thA;
	B thB;

	thA.start();
	thB.start();
	unlock;

	for(int i=0;i<30;i++){
		lock;
		cout<<"main "<<i<<endl;
		unlock;

		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}

	cout<<"HAPPY END!";
	return 0;
}

int main(int argc,char*argv[]){
  changeTimerRoutine();
  userMain(argc,argv);
  restoreTimerRoutine();
  return 0;
}
