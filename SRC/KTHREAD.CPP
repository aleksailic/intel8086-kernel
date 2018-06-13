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
#include <IOSTREAM.H>
#include "SCHEDULE.H"
#include "SLIST.H"
#include "THREAD.H"

#define GETPARAM(type,id) *((type*)data->params[id])
#define VEC_CAP 32

void KernelThread::_sleep(SysData* data){
	//PCB RUNNING JE KERNEL NIT!, blokirana nit mora da pajki
	KernelThread::blocked->toSleep= GETPARAM(unsigned,0);
	SET_FLAG(KernelThread::blocked,PCB_SLEEPING);

	Timer::sleeplist->add(KernelThread::blocked);

	//moramo promeniti kontekst na sledecu dozvoljenu, inace nece doci do spavanja uopste
	KernelThread::blocked=Scheduler::get();
	if(KernelThread::blocked==0)
		KernelThread::blocked=PCB::nop;
}
void KernelThread::_dispatch(SysData*data){
	if(!CHK_KFLAG(PCB_FINISHED) && !CHK_KFLAG(PCB_SLEEPING) && !CHK_KFLAG(PCB_BLOCKED) && !CHK_KFLAG(PCB_NOP) && !CHK_KFLAG(PCB_KERNEL))
		Scheduler::put((PCB*)KernelThread::blocked);
	KernelThread::blocked=Scheduler::get();
	if(KernelThread::blocked==0)
		KernelThread::blocked=PCB::nop;
}
void KernelThread::_thread_create(SysData* data){
	ID*id=GETPARAM(ID*,3);
	*id=runningKernelThread->pcbs.add(
		new PCB(
			GETPARAM(StackSize,0),
			GETPARAM(Time,1),
			GETPARAM(Thread*,2)
		)
	);
}
void KernelThread::_thread_start(SysData* data){
	cout<<"U scheduler stavljen PCB "<<GETPARAM(ID,0)<<'\n';
	Scheduler::put(runningKernelThread->pcbs[GETPARAM(ID,0)]);
}
void KernelThread::_thread_join(SysData* data){
	//sys_sem_wait();
}
void KernelThread::_sem_wait(SysData* data){}
void KernelThread::_sem_signal(SysData* data){}
void KernelThread::_event_wait(SysData* data){}
void KernelThread::_event_signal(SysData* data){}

KernelThread* KernelThread::self=0;
KernelThread* runningKernelThread=KernelThread::getKernelThread(); // initialize kernel thread

const IVTNo KernelThread::ivtNo=0x60; //adresa prekidne rutine koja je slobodna
unsigned KernelThread::task = SET_KERNEL;

KernelThread::KernelThread():pcbs(VEC_CAP),semaphores(VEC_CAP),events(VEC_CAP){
	myPCB=new PCB(defaultStackSize,0,0,PCB_KERNEL,0,triggerRun); //id:0, nema thread jer ovo nije thread zapravo

	pcbs[0]=myPCB;
	pcbs[1]=PCB::running;
	pcbs[2]=PCB::nop;

	syscallfn[SLEEP]=_sleep;
	syscallfn[DISPATCH]=_dispatch;
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

static unsigned tcx;
static unsigned tdx;
static unsigned tbp;
static unsigned tss;
static unsigned tsp;
static unsigned segFn=FP_SEG(KernelThread::triggerRun);
static unsigned offFn=FP_OFF(KernelThread::triggerRun);
PCB* KernelThread::blocked = 0;

#define PUSH_FN()\
	asm pushf;\
	asm push segFn;\
	asm push offFn;\
	asm sub sp, 9

#define RESTORE_KERNEL_STACK(pcb,seg,off)\
    pcb->stack[pcb->scap-1]=0x200;\
    pcb->stack[pcb->scap-2]=seg;\
    pcb->stack[pcb->scap-3]=off;\
    pcb->sp=FP_OFF(pcb->stack+pcb->scap-12);\
    pcb->ss=FP_SEG(pcb->stack+pcb->scap-12);\
    pcb->bp=FP_OFF(pcb->stack+pcb->scap-12)

void interrupt KernelThread::routine(...){
	asm{
		cli
		mov tcx, cx
		mov tdx, dx
	}
	STORE_REGS(PCB::running);
	CLR_RFLAG(PCB_RUNNING);

	if(task==SET_KERNEL){ //odi na kernel stek
		if(!CHK_RFLAG(PCB_KERNEL)){
			SET_RFLAG(PCB_BLOCKED);
			blocked=PCB::running;
			PCB::running=runningKernelThread->myPCB;
		}else{
			runningKernelThread->nestcalls.add(new NestCall(tsp,tss,tbp));
			PUSH_FN();
			STORE_REGS(PCB::running);
		}
		//cout<<"pcb"<<PCB::running->id <<" trazi kernel\n";

	}else if(task==FINISH_KERNEL){
		if(runningKernelThread->nestcalls.empty()){
			PCB::running=blocked;
			blocked=0;
			RESTORE_KERNEL_STACK(runningKernelThread->myPCB,segFn,offFn);
		}else{
			RESTORE_REGS(runningKernelThread->nestcalls.get());
		}
		//cout<<"kernel vraca pcb"<<PCB::running->id<<"\n";
	}

	SET_STATUS_RFLAG(PCB_RUNNING);
	CLR_KTASK();
	Timer::count=PCB::running->quant;
	RESTORE_REGS(PCB::running);
}
void KernelThread::triggerRun(){
	runningKernelThread->run();
	SET_KTASK(FINISH_KERNEL);
	routine(); //push psw,push retaddr, call routine
}
void KernelThread::run(){
	//dozvoljavamo prekide, ali ne damo kontekst!
	Lock::set();
	asm sti;
	//BITNO! Cuva se na kernelskom steku sto omogucava ugnjezdavanje
	SysData* sysdata=(SysData*)MK_FP(tcx,tdx); //Izvlacimo podatke iz registra, sto je mem. lokacija (stek pozivne f-je) potrebn
	syscallfn[sysdata->id](sysdata); //call the function

	//samo ako je poslednji u nizu
	if(nestcalls.empty())
		Lock::unset();
}
