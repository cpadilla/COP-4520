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

