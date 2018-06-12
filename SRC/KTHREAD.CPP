/*
 * KTHREAD.CPP
 *
 *  Created on: Jun 11, 2018
 *      Author: OS1
 */


#include "KTHREAD.H"
#include "PCB.H"
#include "TIMER.H"
#include "SYS.H"
#include <DOS.H>

#define GETPARAM(type,id) *((type*)sysdata->params[id])

static void _sleep(SysData* data){}
static void _thread_create(SysData* data){}
static void _thread_join(SysData* data){}
static void _thread_start(SysData* data){}
static void _sem_wait(SysData* data){}
static void _sem_signal(SysData* data){}
static void _event_wait(SysData* data){}
static void _event_signal(SysData* data){}

KernelThread* KernelThread::self=0;
KernelThread* runningKernelThread=KernelThread::getKernelThread(); // initialize kernel thread

const IVTNo KernelThread::ivtNo=0x60; //adresa prekidne rutine koja je slobodna
unsigned KernelThread::task = SET_KERNEL;

KernelThread::KernelThread(){
	myPCB=new PCB(defaultStackSize,0,0,PCB_KERNEL,0,triggerRun); //id:0, nema thread jer ovo nije thread zapravo
	syscallfn[SLEEP]=_sleep;
	syscallfn[THREAD_CREATE]=_thread_create;
	syscallfn[THREAD_JOIN]=_thread_join;
	syscallfn[THREAD_START]=_thread_start;
	syscallfn[SEM_WAIT]=_sem_wait;
	syscallfn[SEM_SIGNAL]=_sem_signal;
	syscallfn[EVENT_SIGNAL]=_event_signal;
	syscallfn[EVENT_WAIT]=_event_wait;

	setvect(ivtNo, routine); // ne mora da je vracamo posto je ovo slobodna zona
}
KernelThread* KernelThread::getKernelThread(){
	if(self==0)
		self=new KernelThread();
	return self;
}

static unsigned tcx,tdx;
static unsigned segFn=FP_SEG(KernelThread::triggerRun);
static unsigned offFn=FP_OFF(KernelThread::triggerRun);
PCB* KernelThread::blocked = 0;

#define PUSH_FN()\
	asm push offFn;\
	asm push segFn;\
	asm pushf

void interrupt KernelThread::routine(...){
	STORE_REGS(PCB::running);
	CLR_RFLAG(PCB_RUNNING);
	if(CHK_KTASK(SET_KERNEL)){
		if(!CHK_RFLAG(PCB_KERNEL)){ //da li nismo na kernel steku?
			SET_RFLAG(PCB_BLOCKED);
			blocked=PCB::running;
			PCB::running=runningKernelThread->myPCB;
			RESTORE_REGS(PCB::running);
		}else{
			runningKernelThread->nestcalls.add(new NestCall(tsp,tss,tbp));
			PUSH_FN();
		}
	}
	else if(CHK_KTASK(FINISH_KERNEL)){
		if(runningKernelThread->nestcalls.empty()){ //vracamo se na blokiranu nit
			PCB::running=blocked;
			CLR_RFLAG(PCB_BLOCKED);
			RESTORE_REGS(PCB::running);
		}else{
			RESTORE_REGS(runningKernelThread->nestcalls.get());
		}
	}
	SET_RFLAG(PCB_RUNNING);
	CLR_KTASK();
}
void KernelThread::triggerRun(){
	runningKernelThread->run();
	SET_KTASK(FINISH_KERNEL);
	routine(); //push retaddr, push psw , call routine
}
void KernelThread::run(){
	//dozvoljavamo prekide, ali ne damo kontekst!
	Lock::set();
	asm{
		mov tcx,cx
		mov tdx,dx
		sti
	}
	//BITNO! Cuva se na kernelskom steku sto omogucava ugnjezdavanje
	SysData* sysdata=(SysData*)MK_FP(tcx,tdx); //Izvlacimo podatke iz registra, sto je mem. lokacija (stek pozivne f-je) potrebn
	syscallfn[sysdata->id](sysdata); //call the function

	//samo ako je poslednji u nizu
	if(nestcalls.empty())
		Lock::unset();
}
