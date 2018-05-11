#include <IOSTREAM.H>
#include <DOS.H>

#include "UTILS.H"
#include "TIMER.H"
#include "PCB.H"

// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

void dispatch(){ // sinhrona promena konteksta
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

void exitThread(){
	running->zavrsio = 1;
	dispatch();
}

void a(){
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"u a() i = "<<i<<endl;
		unlock
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"u b() i = "<<i<<endl;
		unlock
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}


void createProcess(PCB *newPCB, void (*body)()){
	unsigned* st1 = new unsigned[1024];

	st1[1023] =0x200;//setovan I fleg u
                      // pocetnom PSW-u za nit
	st1[1022] = FP_SEG(body);
	st1[1021] = FP_OFF(body);

newPCB->sp = FP_OFF(st1+1012); //svi sacuvani registri
 //pri ulasku u interrupt
 //rutinu
	newPCB->ss = FP_SEG(st1+1012);
	newPCB->zavrsio = 0;
}

int userMain(int argc,char*argv[]){
	lock
	p[1] = new PCB();
	createProcess(p[1],a);
	cout<<"napravio a"<<endl;
	p[1]->kvant = 40;

	p[2] = new PCB();
	createProcess(p[2],b);
	cout<<"napravio b"<<endl;
	p[2]->kvant = 20;

	p[0] = new PCB();

	running = p[0];
	unlock

	for (int i = 0; i < 30; ++i) {
  	lock
  	cout<<"main "<<i<<endl;
  	unlock

	for (int j = 0; j< 30000; ++j)
		for (int k = 0; k < 30000; ++k);
	}
	cout<<"Happy End"<<endl;
}

int main(int argc,char*argv[]){
  changeTimerRoutine();
  userMain(argc,argv);
  restoreTimerRoutine();
  return 0;
}

