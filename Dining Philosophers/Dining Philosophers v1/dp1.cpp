/**
*
*Author: Christofer Padilla
*
*Reference: https://code.msdn.microsoft.com/windowsdesktop/Dining-Philosophers-in-C-11-f6bb06a8
*
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <atomic>
 
using namespace std;

atomic_bool dilema = true;

class Chopstick {
public:
	Chopstick() {};
	mutex m;
	int id;
};

int main()
{

	auto philosopher = [](int id, Chopstick* left_chopstick, Chopstick* right_chopstick) {
		string dining = "Philosopher " + to_string(id) + " is now dining.\n";
		cout << dining.c_str();

		if (left_chopstick == right_chopstick)
			throw exception("Left and right chopsticks should not be the same!");

			while (dilema) {
			{
				string hungry = "Philosopher " + to_string(id) + " is now hungry.\n";
				cout << hungry.c_str();

				lock(left_chopstick->m, right_chopstick->m);
				lock_guard<mutex> lock1(left_chopstick->m, adopt_lock);
				string pickupLeft = "Philosopher " + to_string(id) + " picks up chopstick " + to_string(left_chopstick->id) + ".\n";
				cout << pickupLeft.c_str();

				lock_guard<mutex> lock2(right_chopstick->m, adopt_lock);
				string pickupRight = "Philosopher " + to_string(id) + " picks up chopstick " + to_string(right_chopstick->id) + ".\n";
				cout << pickupRight.c_str();

				string eats = "Philosopher " + to_string(id) + " is now eating.\n";
				cout << eats.c_str();
			}

			string thinks = "Philosopher " + to_string(id) + " is now thinking.\n";
			cout << thinks.c_str();
		}
	};

	cout << "Creating chopsticks.\n";
	// Create 5 chopsticks
    Chopstick* chopstick = new Chopstick[5];
    for (int i=0; i<5; i++) {
		chopstick[i].id = i+1;
    }

	cout << "Creating philosophers.\n";
	// Create 5 philosopher threads
	int philosophers = 5;
    thread* threads = new thread[philosophers];
	threads[0] = thread(philosopher, 1, &chopstick[4], &chopstick[0]);
    for (int i=1; i<philosophers; i++) {
        threads[i] = thread(philosopher, (i+1), &chopstick[i-1], &chopstick[i]);
    }

    for (int i=0; i<philosophers; i++) {
        threads[i].join();
    }

    return 0;
}

