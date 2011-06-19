// File: latch_test.cpp
// Written by Joshua Green

#include "latch.h"
#include "suspend.h"
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
using namespace std;

latch my_lock;

const LATCH_PRIORITIES::PRIORITY PRIORITIES[] = { LATCH_PRIORITIES::LOWEST, LATCH_PRIORITIES::LOW, LATCH_PRIORITIES::MEDIUM, LATCH_PRIORITIES::HIGH, LATCH_PRIORITIES::HIGHEST };

void *latch_test_high_priority(void* p) {
    for (int i=0;i<100;i++) {

        my_lock.lock(LATCH_PRIORITIES::HIGHEST);
        cout << "[HIGHEST] Locked." << endl;
        cout.flush();

        suspend(0);

        my_lock.unlock();
        cout << "[HIGHEST] UnLocked." << endl;
        cout.flush();
    }
}

void *latch_test_low_priority(void* p) {
    for (int i=0;i<100;i++) {

        my_lock.lock(LATCH_PRIORITIES::LOWEST);
        cout << "[LOWEST] Locked." << endl;
        cout.flush();

        suspend(1000);

        my_lock.unlock();
        cout << "[LOWEST] UnLocked." << endl;
        cout.flush();
    }
}

void *latch_test(void* p) {
    for (int i=0;i<100;i++) {

        my_lock.lock(PRIORITIES[rand()%5]);
        cout << "Locked." << endl;
        cout.flush();

        suspend(0);

        my_lock.unlock();
        cout << "UnLocked." << endl;
        cout.flush();
    }
}

int main() {
    srand(time(0));
    cout << "Created lock." << endl;
    cout.flush();

    pthread_t t1, t2, t3, t4, t5;
    pthread_attr_t pthread_custom_attr;
    pthread_attr_init(&pthread_custom_attr);

    pthread_create(&t1, 0, latch_test_low_priority, (void*)0);
    pthread_create(&t2, 0, latch_test_high_priority, (void*)0);
    pthread_create(&t3, 0, latch_test_low_priority,  (void*)0);
    pthread_create(&t4, 0, latch_test_high_priority, (void*)0);
    pthread_create(&t5, 0, latch_test_high_priority, (void*)0);

    pthread_join(t1, 0);
    pthread_join(t2, 0);
    pthread_join(t3, 0);
    pthread_join(t4, 0);
    pthread_join(t5, 0);

    char c;
    cin >> c;

    return 0;
}
