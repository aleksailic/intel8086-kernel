/*
 * MIDWAY.CPP
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "T_SEMPC.H"
#include <IOSTREAM.H>
#include <STDLIB.H>

void TSemProdCons::BoundedBuffer::put(int elem){
	spaceAvail.wait(1);
	mutex.wait(1);
	rear=(rear+1)%10;
	data[rear]=elem;
	cout<<"Stavio: "<<elem<<'\n';
	itemAvail.signal();
	mutex.signal();
}

int TSemProdCons::BoundedBuffer::get(){
	itemAvail.wait(1);
	mutex.wait(1);
	front=(front+1)%10;
	cout<<"Uzeo: "<<data[front]<<'\n';
	spaceAvail.signal();
	mutex.signal();
	return data[front];
}

void TSemProdCons::Producer::run(){
	int iter=10;
	for(int i=0;i<iter;i++){
		buf->put(elem);
		Thread::sleep(rand() % 2);
	}
}

void TSemProdCons::Consumer::run(){
	int iter=10;
	for(int j=0;j<iter;j++){
		elem=buf->get();
		Thread::sleep(rand() % 10);
	}
}

void TSemProdCons::run(){
	BoundedBuffer buffer;

	Producer producer1(&buffer,4);
	Producer producer2(&buffer,6);
	Consumer consumer(&buffer);

	producer1.start();
	producer2.start();
	consumer.start();

	producer1.waitToComplete();
	producer2.waitToComplete();
	consumer.waitToComplete();
}
