#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

void foo(int i){
    ostringstream oss;
    oss << "I am thread " << i << endl;
    cout << oss.str();
}

int main()
{
    thread* threads = new thread[8];
    for (int i=0; i<8; i++) {
        threads[i] = thread(foo,i);
    }

    // Wait for threads to finish
    for (int i=0; i<8; i++) {
        threads[i].join();
    }
    cout << "done" << endl;
    return 0;
}
