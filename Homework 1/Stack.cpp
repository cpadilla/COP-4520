#include "stdafx.h"
#include "Stack.h"

template<class T>
Stack<T>::Stack() {
	// new ReentrantLock()
	//this->head = new atomic<Node<T>>(NULL);
}

template<class T>
Stack<T>::~Stack() {

}

template<class T>
void Stack<T>::push(T x) {
	Node<T>* n = new Node<T>(x);
	head = n;
	while (true) {
		if (tryPush(node)) {
			numOps++;
			return;
		}
		else {
			backoff.backoff();
		}
	}
}

template<class T>
bool Stack<T>::tryPush(Node<T>* node) {
	Node<T>* oldTop = head;
	node->next = oldTop;
	return head->compare_exchange_weak(oldTop, node);
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
			backoff.backoff();
		}
	}
}

template<class T>
Node<T>* Stack<T>::tryPop()
{
	Node<T>* oldTop = head;
	if (oldTop == null) {
		throw string("The top of the stack was empty!");
	}

	Node<T>* newTop = oldTop->next;
	if (head->compare_exchange_weak(oldTop, newTop)) {
		return oldTop;
	}
	else {
		return null;
	}
}

template<class T>
int Stack<T>::getNumOps() {
	return numOps;
}

template<class T>
Node<T>* Stack<T>::peek()
{
	return head;
}