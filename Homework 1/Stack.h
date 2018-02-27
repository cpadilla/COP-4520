#pragma once

#include <string>
#include <mutex>
#include <atomic>
#include "Node.h"
#include "Backoff.h"

using namespace std;

template<class T>
class Stack{
private:
	// lock
	//mutex mtx;
	// Node

	using Obj = Node<T>;
	atomic<Obj*> head;
	atomic<int> numOps = 0;

	static const int MIN_DELAY = 100;
	static const int MAX_DELAY = 5000;

	Backoff* backoff = new Backoff(MIN_DELAY, MAX_DELAY);

protected:
	bool tryPush(Node<T>* x);
	Node<T>* tryPop();

public:
	Stack<T>();
	~Stack<T>();

	void push(T x);
	T pop();

	Node<T>* peek();

	int getNumOps();
};

template<class T>
Stack<T>::Stack() {
}

template<class T>
Stack<T>::~Stack() {

}

template<class T>
void Stack<T>::push(T x) {
	Node<T>* node = new Node<T>(x);
	//head = node;
	while (true) {
		if (tryPush(node)) {
			numOps++;
			return;
		}
		else {
			backoff->backoff();
		}
	}
}

template<class T>
bool Stack<T>::tryPush(Node<T>* node) {
	Node<T>* oldTop = head;
	node->next = oldTop;
	return head.compare_exchange_weak(oldTop, node, memory_order_acquire);
}

template<class T>
int Stack<T>::getNumOps() {
	return numOps;
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
	Node<T>* oldTop = head.load();
	if (oldTop == NULL) {
		throw string("The top of the stack was empty!");
	}

	Node<T>* newTop = oldTop->next;
	//printf("tryPop: oldTop: %d newTop: %d\n", oldTop->value, newTop->value);
	if (head.compare_exchange_weak(oldTop, newTop, memory_order_acquire)) {
		return oldTop;
	}
	else {
		return NULL;
	}
}

template<class T>
Node<T>* Stack<T>::peek()
{
	return head;
}