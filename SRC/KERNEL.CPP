#include <IOSTREAM.H>
#include <DOS.H>

#include "TIMER.H"
#include "UTILS.H"

#include "QUEUE.H"
#include "SEMAPHOR.H"

#include <STDLIB.H>

#define lock asm cli
#define unlock asm sti

class BoundedBuffer: public Queue{
	Semaphore mutex;
	Semaphore itemAvail;
	Semaphore spaceAvail;
	int data[10];
	int front,rear;
public:
	BoundedBuffer():mutex(1),itemAvail(0),spaceAvail(10){
		front=rear=0;
	}
	void put(int elem){
		spaceAvail.wait(1);
		mutex.wait(1);
		rear=(rear+1)%10;
		data[rear]=elem;
		cout<<"Stavio: "<<elem<<'\n';
		itemAvail.signal();
		mutex.signal();
	}
	int get(){
		itemAvail.wait(1);
		mutex.wait(1);
		front=(front+1)%10;
		cout<<"Uzeo: "<<data[front]<<'\n';
		spaceAvail.signal();
		mutex.signal();
		return data[front];

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
		buf->put(elem);
		Thread::sleep(rand() % 5);
	}
}
void Consumer::run(){
	int iter=10;
	for(int j=0;j<iter;j++){
		elem=buf->get();
		Thread::sleep(rand() % 10);
	}
}

int userMain(int argc, char*argv[]){
	lock;
	BoundedBuffer buffer;

	Producer producer1(&buffer,4);
	Producer producer2(&buffer,6);
	Consumer consumer(&buffer);

	producer1.start();
	producer2.start();
	consumer.start();

	unlock;

	producer1.waitToComplete();
	producer2.waitToComplete();
	consumer.waitToComplete();

	return 0;
}

int main(int argc,char*argv[]){
	changeTimerRoutine();
	userMain(argc,argv);
	restoreTimerRoutine();
	cout<<"HAPPY END!";
	return 0;
}
