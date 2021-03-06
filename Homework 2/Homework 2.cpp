// Homework 2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Homework 2.h"

using namespace std;
using namespace chrono;

template<class T>
static void popFromStack(int threadNum, Stack<T>* stack) {
	// Sleep for a random amount of time (0 - 5 seconds)
	this_thread::sleep_for(seconds(rand() % 2));
	printf("Starting thread %d.\n", threadNum);

	int i = 0;
	while (i<10) {
		// If our head is empty, push a new value onto the stack
		try {
			int topOfStack = (int)stack->pop();
			printf("POP value: %d | Size: %d.\n", topOfStack, stack->size());
			i++;
		}
		catch (...) {
			printf("Stack was empty!\n");
		}
		this_thread::sleep_for(seconds(rand() % 2));
	}
}

template<class T>
static void pushOntoStack(int threadNum, Stack<T>* stack) {
	printf("Staring queue thread...\n");

	int i = 0;
	while (i < 30) {
		int randomNumber = rand() % 100;
		stack->push(randomNumber);
		printf("PUSH value: %d | Size: %d.\n", randomNumber, stack->size());
		this_thread::sleep_for(milliseconds(rand() % 1000));
		i++;
	}
}

int main()
{
	Stack<int>* stack = new Stack<int>();

	thread a(pushOntoStack<int>, 0, stack);
	thread b(popFromStack<int>, 1, stack);
	thread c(popFromStack<int>, 2, stack);
	thread d(popFromStack<int>, 3, stack);

	a.join();
	b.join();
	c.join();
	d.join();

	Node<int>* node = stack->peek();
	while (node != NULL) {
		cout << node->value << " -> ";
		node = node->next;
	}

	cout << "Done...";

	this_thread::sleep_for(seconds(60));

	return 0;
}
