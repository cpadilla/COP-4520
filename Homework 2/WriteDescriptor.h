#pragma once

#include <atomic>

using namespace std;

template<class T>
class WriteDescriptor {
public:
	Node<T>* old_value;
	Node<T>* new_value;
	atomic<bool> pending{false};
};

