#pragma once

#include <string>
#include <mutex>
#include <atomic>
#include "Node.h"
#include "Backoff.h"
#include "RDCSS.h"

using namespace std;

static const int MIN_DELAY = 100;
static const int MAX_DELAY = 5000;

template<class T>
class Stack{
private:
	RDCSS<T>* rdcss = new RDCSS<T>();

	atomic<RDCSSDescriptor<T>*> descriptor;
	using Obj = Node<T>;

	Backoff* backoff = new Backoff(MIN_DELAY, MAX_DELAY);

protected:
	bool tryPush(RDCSSDescriptor<T>* desc, RDCSSDescriptor<T>* newdesc);
	Node<T>* tryPop();

public:
	atomic<Obj*> head{NULL};
	atomic<int> _size{ 0 };

	Stack<T>();
	~Stack<T>();

	void push(T x);
	T pop();

	int size();
};

template<class T>
Stack<T>::Stack() {
	descriptor = new RDCSSDescriptor<T>();
}

template<class T>
Stack<T>::~Stack() {

}

template<class T>
void Stack<T>::push(T x) {
	Node<T>* node = new Node<T>(x);
	while (true) {
		RDCSSDescriptor<T>* desc = descriptor.load();

		RDCSSDescriptor<T>* newdesc = new RDCSSDescriptor<T>();

		//complete_write(desc->writeop);
		rdcss->complete(desc);

		if (tryPush(desc, newdesc)) {
			//complete_write(newdesc->writeop);
			rdcss->complete(newdesc);
			return;
		}
		else {
			backoff->backoff();
		}
	}
}

template<class T>
bool Stack<T>::tryPush(RDCSSDescriptor<T>* desc, RDCSSDescriptor<T>* newdesc) {
	bool value = false;

	value = descriptor.compare_exchange_weak(desc, newdesc, memory_order_acquire);

	return value;
}

template<class T>
inline int Stack<T>::size()
{
	//RDCSSDescriptor<T>* desc = descriptor.load();
	return 0;
}

template<class T>
T Stack<T>::pop() {
	while (true) {
		Node<T>* returnNode = tryPop();
		if (returnNode != NULL) {
			return returnNode->value;
		}
		else {
			backoff->backoff();
		}
	}
}

template<class T>
Node<T>* Stack<T>::tryPop()
{

	RDCSSDescriptor<T>* desc = descriptor.load();

	//complete_write(desc->writeop);

	Node<T>* oldTop = head.load();
	if (oldTop == NULL) {
		throw string("The top of the stack was empty!");
	}

	if (descriptor.compare_exchange_weak(desc, newdesc, memory_order_acquire)) {
		complete_write(newdesc->writeop);
		return oldTop;
	}
	else {
		return NULL;
	}
}

