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
	//cout << "Staring thread " << threadNum << " ...\n";
	printf("Starting thread %d.\n", threadNum);

	while (true) {
		//if (stack->peek() == NULL) {
			// If our head is empty, push a new value onto the stack
			int topOfStack = stack->pop();
			//string popoff = "Popped value: " + topOfStack + "\n";
			printf("Popped value: %d.\n", topOfStack);
		//}
		this_thread::sleep_for(seconds(rand() % 2));
		if (stack->getNumOps() > 20) return;
	}
}

template<class T>
static void pushOntoStack(int threadNum, Stack<T>* stack) {
	printf("Staring queue thread...\n");

	while (true) {
		int randomNumber = rand() % 100;
		stack->push(randomNumber);
		//mtx.lock();
		printf("Pushed value: %d.\n", randomNumber);
		//mtx.unlock();
		this_thread::sleep_for(milliseconds(rand() % 1000));
		if (stack->getNumOps() > 20) return;
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
	// new ReentrantLock()
	//using Obj = Node<T>;
	//this->head = new atomic<Obj>(NULL);
}

template<class T>
Stack<T>::~Stack() {

}

template<class T>
void Stack<T>::push(T x) {
	Node<T>* node = new Node<T>();
	node->value = x;
	head = node;
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
	return head.compare_exchange_weak(oldTop, node);
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
	Node<T>* oldTop = head;
	if (oldTop == NULL) {
		throw string("The top of the stack was empty!");
	}

	Node<T>* newTop = oldTop->next;
	if (head.compare_exchange_weak(oldTop, newTop)) {
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
