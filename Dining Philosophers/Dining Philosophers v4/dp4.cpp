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
#include <cstdlib>
 
using namespace std;

atomic_bool dilema;

class Chopstick {
public:
	Chopstick() {};
	mutex m;
	int id;
};

int main(int argc, char* argv[])
{

	dilema = true;

	auto philosopher = [](int id, Chopstick* left_chopstick, Chopstick* right_chopstick) {

		if (left_chopstick == right_chopstick)
			throw runtime_error("Left and right chopsticks should not be the same!");

		while (dilema) {
			string hungry = "Philosopher " + to_string(id) + " is now hungry.\n";
			cout << hungry.c_str();
			bool isHungry = true;

			while (isHungry) {
				if (id % 2 == 0) {
					// Pickup left before right
					if (left_chopstick->m.try_lock()) {
#ifdef DEBUG
						string pickupLeft = "Philosopher " + to_string(id) + " picks up chopstick " + to_string(left_chopstick->id) + ".\n";
						cout << pickupLeft.c_str();
#endif
					}
					else {
#ifdef DEBUG
						string waitingOnLeft = "Philosopher " + to_string(id) + " is waiting for chopstick " + to_string(left_chopstick->id) + ".\n";
						cout << waitingOnLeft.c_str();
#endif
						continue;
					}

					if (right_chopstick->m.try_lock()) {
#ifdef DEBUG
						string pickupRight = "Philosopher " + to_string(id) + " picks up chopstick " + to_string(right_chopstick->id) + ".\n";
						cout << pickupRight.c_str();
#endif
					}
					else {
#ifdef DEBUG
						string waitingOnRight = "Philosopher " + to_string(id) + " is waiting for chopstick " + to_string(right_chopstick->id) + ".\n";
						cout << waitingOnRight.c_str();
#endif
						left_chopstick->m.unlock();
#ifdef DEBUG
						string releasingChopstick = "Philosopher " + to_string(id) + " releases chopstick " + to_string(left_chopstick->id) + ".\n";
						cout << releasingChopstick.c_str();
#endif
						continue;
					}

					string eats = "Philosopher " + to_string(id) + " is now eating.\n";
					cout << eats.c_str();
					isHungry = false;
				} else {
					// Pickup right before left
					if (right_chopstick->m.try_lock()) {
#ifdef DEBUG
						string pickupRight = "Philosopher " + to_string(id) + " picks up chopstick " + to_string(right_chopstick->id) + ".\n";
						cout << pickupRight.c_str();
#endif
					}
					else {
#ifdef DEBUG
						string waitingOnRight = "Philosopher " + to_string(id) + " is waiting for chopstick " + to_string(right_chopstick->id) + ".\n";
						cout << waitingOnRight.c_str();
#endif
						continue;
					}

					if (left_chopstick->m.try_lock()) {
#ifdef DEBUG
						string pickupLeft = "Philosopher " + to_string(id) + " picks up chopstick " + to_string(left_chopstick->id) + ".\n";
						cout << pickupLeft.c_str();
#endif
					}
					else {
#ifdef DEBUG
						string waitingOnLeft = "Philosopher " + to_string(id) + " is waiting for chopstick " + to_string(left_chopstick->id) + ".\n";
						cout << waitingOnLeft.c_str();
#endif
						right_chopstick->m.unlock();
#ifdef DEBUG
						string releasingChopstick = "Philosopher " + to_string(id) + " releases chopstick " + to_string(right_chopstick->id) + ".\n";
						cout << releasingChopstick.c_str();
#endif
						continue;
					}

					string eats = "Philosopher " + to_string(id) + " is now eating.\n";
					cout << eats.c_str();
					isHungry = false;
				}
			}

			string thinks = "Philosopher " + to_string(id) + " is now thinking.\n";
			cout << thinks.c_str();

			left_chopstick->m.unlock();
#ifdef DEBUG
			string releasingChopstick = "Philosopher " + to_string(id) + " releases chopstick " + to_string(left_chopstick->id) + ".\n";
			cout << releasingChopstick.c_str();
#endif

			right_chopstick->m.unlock();
#ifdef DEBUG
			releasingChopstick = "Philosopher " + to_string(id) + " releases chopstick " + to_string(right_chopstick->id) + ".\n";
			cout << releasingChopstick.c_str();
#endif
		}
	};

	auto waitForInput = []() {
		while (1) {
			char n;
			cin >> n;
			if (n == 'n') {
				dilema = false;
				break;
			}
		}
	};

	if (argc < 2) {
		throw runtime_error("Missing number of philosophers argument!\n");
	}

	int philosophers = atoi(argv[1]);

	if (philosophers < 1) {
		throw runtime_error("Must be at least 1 philosopher!\n");
	}

	// Make sure there are at least 2 chopsticks
	int chopsticks = (philosophers > 1 ? philosophers : 2);

	// Create chopsticks
    Chopstick* chopstick = new Chopstick[chopsticks];
    for (int i=0; i<chopsticks; i++) {
		chopstick[i].id = i+1;
    }

	// Create philosopher threads
    thread* threads = new thread[philosophers];
	if (philosophers == 1) {
		threads[0] = thread(philosopher, 1, &chopstick[1], &chopstick[0]);
	} else {
		threads[0] = thread(philosopher, 1, &chopstick[philosophers-1], &chopstick[0]);
		for (int i=1; i<philosophers; i++) {
			threads[i] = thread(philosopher, (i+1), &chopstick[i-1], &chopstick[i]);
		}
	}

	thread wait = thread(waitForInput);
	wait.join();

    for (int i=0; i<philosophers; i++) {
        threads[i].join();
    }

    return 0;
}

