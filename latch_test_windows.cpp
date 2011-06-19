// File: latch_test.cpp
// Written by Joshua Green

#include "latch.h"
#include "suspend.h"
#include <iostream>
#include <process.h>
#include <cstdlib>
#include <time.h>
using namespace std;

latch my_lock;

const LATCH_PRIORITIES::PRIORITY PRIORITIES[] = { LATCH_PRIORITIES::LOWEST, LATCH_PRIORITIES::LOW, LATCH_PRIORITIES::MEDIUM, LATCH_PRIORITIES::HIGH, LATCH_PRIORITIES::HIGHEST };

void latch_test_high_priority(void* p) {
    for (int i=0;i<50;i++) {

        my_lock.lock(LATCH_PRIORITIES::HIGHEST);
        cout << "[HIGHEST] Locked." << endl;

        suspend(100);

        my_lock.unlock();
        cout << "[HIGHEST] UnLocked." << endl;
    }
}

void latch_test_low_priority(void* p) {
    for (int i=0;i<50;i++) {

        my_lock.lock(LATCH_PRIORITIES::LOWEST);
        cout << "[LOWEST] Locked." << endl;

        suspend(1000);

        my_lock.unlock();
        cout << "[LOWEST] UnLocked." << endl;
    }
}

void latch_test(void* p) {
    for (int i=0;i<100;i++) {

        my_lock.lock(PRIORITIES[rand()%5]);
        cout << "Locked." << endl;

        suspend(0);

        my_lock.unlock();
        cout << "UnLocked." << endl;
    }
}

int main() {
    srand(time(0));
    cout << "Created lock." << endl;

    _beginthread(&latch_test_high_priority, 0, (void*)0);
    _beginthread(&latch_test_high_priority, 0, (void*)0);
    _beginthread(&latch_test_low_priority,  0, (void*)0);
    _beginthread(&latch_test_high_priority, 0, (void*)0);
    _beginthread(&latch_test_high_priority, 0, (void*)0);

    char c;
    cin >> c;

    return 0;
}
