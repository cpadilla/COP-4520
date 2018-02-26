// COP4520 HW1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "COP4520 HW1.h"

using namespace std;
using namespace chrono;

static void foo() {
	this_thread::sleep_for(seconds(5));
	cout << "5 seconds passed!\n";
}

static void bar() {
	this_thread::sleep_for(seconds(3));
	cout << "3 seconds passed!\n";
}

template<class T>
static void popFromStack(int threadNum, Stack<T>* stack) {
	// Sleep for a random amount of time (0 - 5 seconds)
	this_thread::sleep_for(seconds(rand() % 2));
	printf("Starting thread %d.\n", threadNum);

	int i = 0;
	while (i<10) {
		// If our head is empty, push a new value onto the stack
		try {
			int topOfStack = stack->pop();
			//string popoff = "Popped value: " + topOfStack + "\n";
			printf("POP value: %d.\n", topOfStack);
			i++;
		}
		catch (...) {
			printf("Stack was empty!\n");
		}
		this_thread::sleep_for(seconds(rand() % 2));
		//if (stack->getNumOps() > 20) return;
	}
}

template<class T>
static void pushOntoStack(int threadNum, Stack<T>* stack) {
	printf("Staring queue thread...\n");

	int i = 0;
	while (i < 30) {
		int randomNumber = rand() % 100;
		stack->push(randomNumber);
		printf("PUSH value: %d.\n", randomNumber);
		this_thread::sleep_for(milliseconds(rand() % 1000));
		i++;
	}
}

int main()
{
	stack = new Stack<int>();

	thread a(pushOntoStack<int>, 0, stack);
	thread b(popFromStack<int>, 1, stack);
	thread c(popFromStack<int>, 2, stack);
	thread d(popFromStack<int>, 3, stack);

	a.join();
	b.join();
	c.join();
	d.join();

	printf("Done...");

	this_thread::sleep_for(seconds(60));

	return 0;
}

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
int Stack<T>::getNumOps() {
	return numOps;
}

template<class T>
Node<T>* Stack<T>::peek()
{
	return head;
}

template<class T>
Node<T>::Node() {

}

template<class T>
Node<T>::Node(T value)
{
	this->value = value;
	//this->next = NULL;
}
