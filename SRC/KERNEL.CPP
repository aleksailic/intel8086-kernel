#include <IOSTREAM.H>
#include <DOS.H>

#include "TIMER.H"
#include "UTILS.H"

#include "QUEUE.H"
#include "SEMAPHOR.H"

#include <STDLIB.H>

#define lock asm cli
#define unlock asm sti

#define N 10
class BoundedBuffer: public Queue{
	Semaphore mutex;
	Semaphore itemAvail;
	Semaphore spaceAvail;
	int data[N];
	int head,tail;
public:
	BoundedBuffer():mutex(0),itemAvail(0),spaceAvail(N),head(0),tail(0){}
	void put(int elem){
		spaceAvail.wait(1);
		mutex.wait(1);
		tail=(tail+1)%N;
		data[head]=elem;
		itemAvail.signal();
		mutex.signal();
	}
	int get(){
		itemAvail.wait(1);
		mutex.wait(1);
		head=(head+1)%N;
		return data[head];
		spaceAvail.signal();
		mutex.signal();
	}
};

class Producer:public Thread{
	BoundedBuffer*buf;
	void run();
	int elem;
public:
	Producer(BoundedBuffer*buffer,int element=2):buf(buffer),elem(element){}
};
class Consumer:public Thread{
	BoundedBuffer*buf;
	void run();
	int elem;
public:
	Consumer(BoundedBuffer*buffer):buf(buffer){}
};
void Producer::run(){
	int iter=10;
	for(int i=0;i<iter;i++){
		//buf->put(elem);
		lock;
		cout<<"Stavio: "<<elem << endl;
		unlock;
		Thread::sleep(rand()%4);
	}
}
void Consumer::run(){
	int iter=10;
	for(int i=0;i<iter;i++){
		Thread::sleep(rand()%4);
		elem=buf->get();
		lock;
		cout<<"Uzeo: "<<elem << endl;
		unlock;
	}
}

int userMain(int argc, char*argv[]){
	lock;
	cout<<"Pokrece se main\n";


	BoundedBuffer buffer;

	Producer producer1(&buffer,4);
	Producer producer2(&buffer,6);
	//Consumer consumer(&buffer);

	producer1.start();
	producer2.start();
	//consumer.start();


	unlock;

	return 0;
}

int main(int argc,char*argv[]){
	changeTimerRoutine();
	userMain(argc,argv);
	restoreTimerRoutine();
	cout<<"HAPPY END!";
	return 0;
}
