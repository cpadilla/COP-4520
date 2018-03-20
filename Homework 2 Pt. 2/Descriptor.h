#pragma once

#include <atomic>
#include "WriteDescriptor.h"

using namespace std;

template<class T>
class Descriptor {

public:
	WriteDescriptor<T>* writeop;

	atomic<int> size{NULL};
	atomic<int> numOps{NULL};
};
