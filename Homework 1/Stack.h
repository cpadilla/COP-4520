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