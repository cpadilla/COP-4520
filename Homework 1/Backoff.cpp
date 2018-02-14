#include "stdafx.h"
#include "Backoff.h"


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
