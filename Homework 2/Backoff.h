#pragma once
#include <stdlib.h>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

class Backoff
{
private:
	const int minDelay, maxDelay;
	int limit;
public:
	Backoff(int min, int max);
	void backoff();
	~Backoff();
};

Backoff::Backoff(int min, int max) : minDelay(min), maxDelay(max) {
	limit = minDelay;
}

void Backoff::backoff() {
	int delay = rand() % limit;
	limit = min(maxDelay, 2 * limit);
	chrono::milliseconds milli(delay);
	this_thread::sleep_for(milli);
}


Backoff::~Backoff() {
}

