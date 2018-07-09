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
#include "IVTENTRY.H"
#include "KEVENT.H"

#define GETPARAM(type,id) *((type*)data->params[id])
#define VEC_CAP 32

void KernelThread::_sleep(SysData* data){
	//PCB RUNNING JE KERNEL NIT!, blokirana nit mora da pajki
	KernelThread::captured->toSleep= GETPARAM(unsigned,0);
	SET_FLAG(KernelThread::captured,PCB_SLEEPING);

	Timer::sleeplist->add(KernelThread::captured);

	//moramo promeniti kontekst na sledecu dozvoljenu, inace nece doci do spavanja uopste
	//nema smisla koristiti sistemski poziv, kad smo vec na kernel steku
	_dispatch(data);
}
void KernelThread::_dispatch(SysData*data){
	if(!CHK_KFLAG(PCB_FINISHED) && !CHK_KFLAG(PCB_SLEEPING) && !CHK_KFLAG(PCB_BLOCKED) && !CHK_KFLAG(PCB_NOP) && !CHK_KFLAG(PCB_KERNEL))
		Scheduler::put((PCB*)KernelThread::captured);
	KernelThread::captured=Scheduler::get();
	if(KernelThread::captured==0)
		KernelThread::captured=PCB::nop;
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
	asm cli;
	cout<<"U scheduler stavljen PCB "<<GETPARAM(ID,0)<<'\n';
	asm sti;
	Scheduler::put(runningKernelThread->pcbs[GETPARAM(ID,0)]);
}
void KernelThread::_thread_join(SysData* data){
	thread_t id = GETPARAM(thread_t,0);
	runningKernelThread->pcbs[id]->finished.wait(1); //indirektno poziva sys_dispatch()
}
void KernelThread::_thread_destroy(SysData* data){
	thread_t id = GETPARAM(thread_t,0);
	delete runningKernelThread->pcbs[id];
	runningKernelThread->pcbs.remove(id);
}
void KernelThread::_sem_create(SysData* data){
	sem_t * id = GETPARAM(sem_t*,0);
	int val = GETPARAM(int,1);
	*id=runningKernelThread->semaphores.add(new KernelSem(val));
}
void KernelThread::_sem_wait(SysData* data){
	sem_t id = GETPARAM(sem_t,0);
	int toBlock = GETPARAM(int,1);
	int* retVal = GETPARAM(int*,2);
	*retVal = runningKernelThread->semaphores[id]->wait(toBlock);
}
void KernelThread::_sem_signal(SysData* data){
	sem_t id = GETPARAM(sem_t,0);
	runningKernelThread->semaphores[id]->signal();
}
void KernelThread::_sem_val(SysData* data){
	sem_t id = GETPARAM(sem_t,0);
	int* retVal = GETPARAM(int*,1);
	*retVal = runningKernelThread->semaphores[id]->getVal();
}
void KernelThread::_sem_destroy(SysData* data){
	sem_t id = GETPARAM(sem_t,0);
	delete runningKernelThread->semaphores[id];
	runningKernelThread->semaphores.remove(id);
}
void KernelThread::_event_create(SysData* data){
	event_t* id=GETPARAM(event_t*,0);
	IVTNo ivtNo = GETPARAM(IVTNo,1);

	IVTEntry* entry= IVTEntry::fetch(ivtNo);
	if(entry){
		KernelEv* evt = new KernelEv();
		IVTEntry::fetch(ivtNo)->setEvent(evt);
		*id=runningKernelThread->events.add(evt);
	}
	else
		cout<<"GRESKA!";
}
void KernelThread::_event_wait(SysData* data){
	event_t id=GETPARAM(event_t,0);
	runningKernelThread->events[id]->wait();
}
void KernelThread::_event_signal(SysData* data){
	event_t id=GETPARAM(event_t,0);
	runningKernelThread->events[id]->signal();
}
void KernelThread::_event_destroy(SysData* data){
	event_t id=GETPARAM(event_t,0);
	delete runningKernelThread->events[id];
	runningKernelThread->events.remove(id);
}

KernelThread* KernelThread::self=0;
KernelThread* runningKernelThread=KernelThread::getKernelThread(); // initialize kernel thread

const IVTNo KernelThread::ivtNo=0x60; //adresa prekidne rutine koja je slobodna
unsigned KernelThread::task = SET_KERNEL;

fn KernelThread::syscallfn[] = {
	_sleep,_dispatch,
	_thread_create,_thread_start,_thread_join,_thread_destroy,
	_sem_create,_sem_wait,_sem_signal,_sem_val,_sem_destroy,
	_event_create,_event_wait,_event_signal,_event_destroy
};
KernelThread::KernelThread():pcbs(VEC_CAP),semaphores(VEC_CAP),events(VEC_CAP),nestcalls(5){
	myPCB=new PCB(defaultStackSize,0,0,PCB_KERNEL,0,triggerRun); //id:0, nema thread jer ovo nije thread zapravo

	pcbs.add(myPCB);
	pcbs.add(PCB::running);
	pcbs.add(PCB::nop);

	setvect(ivtNo, routine); // ne mora da je vracamo posto je ovo slobodna zona
}
KernelThread* KernelThread::getKernelThread(){
	if(self==0)
		self=new KernelThread();
	return self;
}

static unsigned tcx,tdx,tbp,tss,tsp;
static unsigned Ibit = 0x200;
static unsigned segFn=FP_SEG(KernelThread::triggerRun);
static unsigned offFn=FP_OFF(KernelThread::triggerRun);
PCB* KernelThread::captured = 0;

#define PUSH_FN()\
	asm push Ibit;\
	asm push segFn;\
	asm push offFn;\
	asm sub sp, 18

#define RESTORE_KERNEL_STACK(pcb,seg,off)\
    pcb->stack[pcb->scap-1]=0x200;\
    pcb->stack[pcb->scap-2]=seg;\
    pcb->stack[pcb->scap-3]=off;\
    pcb->sp=FP_OFF(pcb->stack+pcb->scap-12);\
    pcb->ss=FP_SEG(pcb->stack+pcb->scap-12);\
    pcb->bp=FP_OFF(pcb->stack+pcb->scap-12)


static NestCall nestcall;

void interrupt KernelThread::routine(...){
	asm{
		mov tcx, cx
		mov tdx, dx
	}
	STORE_REGS(PCB::running);
	CLR_RFLAG(PCB_RUNNING);

	if(task==SET_KERNEL){ //odi na kernel stek
		if(!CHK_RFLAG(PCB_KERNEL)){
			captured=PCB::running;
			PCB::running=runningKernelThread->myPCB;
		}else{
			nestcall.bp=tbp;
			nestcall.ss=tss;
			nestcall.sp=tsp;
			runningKernelThread->nestcalls.push(nestcall);
			PUSH_FN();
			STORE_REGS(PCB::running);
}

	}else if(task==FINISH_KERNEL){
		if(runningKernelThread->nestcalls.empty()){
			PCB::running=captured;
			captured=0;
			RESTORE_KERNEL_STACK(runningKernelThread->myPCB,segFn,offFn);
		}else{
			nestcall=runningKernelThread->nestcalls.pop();
			PCB::running->sp=nestcall.sp;
			PCB::running->ss=nestcall.ss;
			PCB::running->bp=nestcall.bp;
		}
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
