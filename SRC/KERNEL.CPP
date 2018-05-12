#include <IOSTREAM.H>
#include <DOS.H>

#include "TIMER.H"
#include "UTILS.H"

#include "PCB.H"

#define lock asm cli
#define unlock asm sti

class A: public Thread{
	static unsigned lastId;
	unsigned id;
	void run();
public:
	A():Thread(1024,5){ id=lastId++; }
};
unsigned A::lastId=0;
void A::run(){
	for (int i =0; i <5; i++) {
		lock;
		cout<<"u a"<< id <<"() i = "<<i<<endl;
		unlock;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

int userMain(int argc, char*argv[]){
	lock;
	cout<<"Pokrece se main\n";
	A thA[10];

	for(int j=0;j<10;j++)
		thA[j].start();

	unlock;

	for(int i=0;i<50;i++){
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
