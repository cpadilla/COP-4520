#pragma once

#include <string>
#include <mutex>
#include <atomic>
#include "Node.h"
#include "Backoff.h"
#include "Descriptor.h"
#include "WriteDescriptor.h"

using namespace std;

static const int MIN_DELAY = 100;
static const int MAX_DELAY = 5000;

enum Operation {
	PUSH,
	POP
};

template<class T>
class Stack{
private:

	atomic<Descriptor<T>*> descriptor;
	using Obj = Node<T>;
	atomic<Obj*> head{NULL};
	atomic<int> numOps{0};

	Backoff* backoff = new Backoff(MIN_DELAY, MAX_DELAY);

protected:
	bool tryPush(Descriptor<T>* desc, Descriptor<T>* newdesc);
	Node<T>* tryPop();
	void complete_write(WriteDescriptor<T>* desc);
	WriteDescriptor<T>* createWriteDesc(Node<T>* head, Node<T>* node, Operation op);
	Descriptor<T>* createDesc(Descriptor<T>* desc, WriteDescriptor<T>* writeOp, int size);

public:
	Stack<T>();
	~Stack<T>();

	void push(T x);
	T pop();

	Node<T>* peek();

	int getNumOps();
	int size();
};

template<class T>
Stack<T>::Stack() {
	descriptor = new Descriptor<T>();
}

template<class T>
Stack<T>::~Stack() {

}

template<class T>
void Stack<T>::push(T x) {
	Node<T>* node = new Node<T>(x);
	while (true) {
		Descriptor<T>* desc = descriptor.load();

		complete_write(desc->writeop);

		WriteDescriptor<T>* writeOp = createWriteDesc(head.load(), node, PUSH);
		Descriptor<T>* newdesc = createDesc(desc, writeOp, desc->size + 1);

		if (tryPush(desc, newdesc)) {
			complete_write(newdesc->writeop);
			return;
		}
		else {
			backoff->backoff();
		}
	}
}

template<class T>
bool Stack<T>::tryPush(Descriptor<T>* desc, Descriptor<T>* newdesc) {
	bool value = false;

	value = descriptor.compare_exchange_weak(desc, newdesc, memory_order_acquire);

	return value;
}

template<class T>
int Stack<T>::getNumOps() {
	Descriptor<T>* desc = descriptor.load();
	return desc->numOps;
}

template<class T>
inline int Stack<T>::size()
{
	Descriptor<T>* desc = descriptor.load();
	return desc->size;
}

template<class T>
T Stack<T>::pop() {
	while (true) {
		Node<T>* returnNode = tryPop();
		if (returnNode != NULL) {
			numOps++;
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

	Descriptor<T>* desc = descriptor.load();

	complete_write(desc->writeop);

	Node<T>* oldTop = head.load();
	if (oldTop == NULL) {
		throw string("The top of the stack was empty!");
	}

	WriteDescriptor<T>* writeOp = createWriteDesc(head.load(), NULL, POP);
	Descriptor<T>* newdesc = createDesc(desc, writeOp, desc->size - 1);

	if (descriptor.compare_exchange_weak(desc, newdesc, memory_order_acquire)) {
		complete_write(newdesc->writeop);
		return oldTop;
	}
	else {
		return NULL;
	}
}

template<class T>
void Stack<T>::complete_write(WriteDescriptor<T>* writeOp)
{
	if (writeOp != NULL && writeOp->pending.load()) {
		head.compare_exchange_weak(writeOp->old_value, writeOp->new_value, memory_order_acquire);
		writeOp->pending.store(false);
	}
}

template<class T>
WriteDescriptor<T>* Stack<T>::createWriteDesc(Node<T>* head, Node<T>* node, Operation op)
{
	WriteDescriptor<T>* writeOp = new WriteDescriptor<T>();

	writeOp->pending.store(true);

	switch (op) {
		case PUSH:
			writeOp->old_value = head;
			node->next = writeOp->old_value;
			writeOp->new_value = node;
			break;
		case POP:
			writeOp->old_value = head;
			writeOp->new_value = head->next;
			break;
	}

	return writeOp;
}

template<class T>
Descriptor<T>* Stack<T>::createDesc(Descriptor<T>* desc, WriteDescriptor<T>* writeOp, int size)
{
	Descriptor<T>* newdesc = new Descriptor<T>();

	newdesc->numOps = desc->numOps + 1;
    newdesc->size = size;
    newdesc->writeop = writeOp;

	return newdesc;
}

template<class T>
Node<T>* Stack<T>::peek()
{
	return head;
}