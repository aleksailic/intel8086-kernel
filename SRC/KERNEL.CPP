#include <IOSTREAM.H>
#include <DOS.H>

#include "TIMER.H"
#include "UTILS.H"

#include <STDLIB.H>

#define lock asm cli
#define unlock asm sti

class A: public Thread{
	static unsigned lastId;
	unsigned id;
	void run();
public:
	A():Thread(1024,10){ id=lastId++; }
};
unsigned A::lastId=0;
void A::run(){
	for (int i =0; i <8; i++) {
		lock;
		cout<<"u a"<< id <<"() i = "<<i<<endl;
		unlock;
		Thread::sleep(rand() % 10);
	}
}

int userMain(int argc, char*argv[]){
	lock;
	cout<<"Pokrece se main\n";
	A thA[4];

	for(int j=0;j<4;j++)
		thA[j].start();

	unlock;

	for(int i=0;i<20;i++){
		lock;
		cout<<"main "<<i<<endl;
		unlock;

		Thread::sleep(4);
	}
	cout<<"EXIT MAIN!";
	return 0;
}

int main(int argc,char*argv[]){
	changeTimerRoutine();
	userMain(argc,argv);
	restoreTimerRoutine();
	cout<<"HAPPY END!";
	return 0;
}
