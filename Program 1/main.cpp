#include <iostream>
#include <thread>
#include <sstream>
#include <atomic>
#include <math.h>
#include <ctime>
#include <vector>
#include <mutex>
#include <algorithm>

using namespace std;

atomic_long number = 0;
long counter;
atomic<double> sum = 0;
vector<long> primes;
mutex store_mutex;

long getNextNumber() {
	return ++number;
}

void incrementCounter() {
	counter++;
}

void addToSum(long number) {
	sum = sum + number;
}

void storePrimes(long number) {
	// lock the mutex for the duration of the scope
	lock_guard<mutex> lock(store_mutex);
	bool add = false;
	
	// add the number if it's greater than any of the primes stored
	for (int i = 0; i < (int) primes.size(); i++) {
		if (number > primes[i]) {
			add = true;
			break;
		}
	}
	if ((primes.size() < 10) || add) {
		primes.push_back(number);
		sort(primes.begin(), primes.end());
		if (primes.size() > 10) {
			primes.erase(primes.begin());
		}
	}
}

bool prime(long n) {
	// check if n is a multiple of 2
	if (n % 2 == 0) return false;
	// if not, then just check the odds
	for (long i = 3; i*i<=n; i+=2)
		if (n%i == 0) return false;
	return true;
}

void run(int threadN){
	long number = 0;
	while ((number = getNextNumber()) <= 10000) {
		if (prime(number)) {
			incrementCounter();
			addToSum(number);
			storePrimes(number);
		}
	}
}

int main()
{
	// Start clock
	int start_s = clock();

	// Start threads
    thread* threads = new thread[8];
    for (int i=0; i<8; i++) {
        threads[i] = thread(run, i);
    }

    // Wait for threads to finish
    for (int i=0; i<8; i++) {
        threads[i].join();
    }
	int stop_s = clock();
    cout << "Execution Time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
    cout << "Total Primes: " << counter << endl;
    cout << "Sum of Primes: " << sum << endl;
    cout << "Top 10 Primes:" << endl;

	for (int i = 0; i < (int) primes.size(); i++) {
		long prime = primes[i];
		cout << (i+1) << ". " << prime << endl;
	}
	system("pause");
    return 0;
}
