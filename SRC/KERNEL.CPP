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
		for (int z = 0; z< 30000; ++z)
				for (int x = 0; x < 30000; ++x);
	}
}
void Consumer::run(){
	int iter=10;
	for(int j=0;j<iter;j++){
		//elem=buf->get();
		lock;
		cout<<"Uzeo: "<<elem<<endl;
		unlock;

		for (int z = 0; z< 30000; ++z)
				for (int k = 0; k < 30000; ++k);
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

	int x=5;

	return 0;
}

int main(int argc,char*argv[]){
	changeTimerRoutine();
	userMain(argc,argv);
	restoreTimerRoutine();
	cout<<"HAPPY END!";
	return 0;
}
