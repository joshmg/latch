// File: latch_test.cpp
// Written by Joshua Green

#include "latch.h"
#include "suspend.h"
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
#include <string>
using namespace std;

latch my_lock;

const unsigned int iterations = 100;
volatile unsigned int count = 0;
volatile unsigned int count_max = 0;

const LATCH_PRIORITIES::PRIORITY PRIORITIES[] = { LATCH_PRIORITIES::LOWEST, LATCH_PRIORITIES::LOW, LATCH_PRIORITIES::MEDIUM, LATCH_PRIORITIES::HIGH, LATCH_PRIORITIES::HIGHEST };
string priority_names[] = { "LOWEST", "LOW", "MEDIUM", "HIGH", "HIGHEST" };

void *latch_test(void* p) {
    for (unsigned int i=0;i<iterations;i++) {
        unsigned long long priority = ((unsigned long long)p);
        if (priority > 4) priority = rand()%5;

        my_lock.lock(PRIORITIES[priority]);
        cout << "[" << priority_names[priority] << "] Locked." << endl;

        ++count;
        //suspend(0);

        my_lock.unlock();
        cout << "[" << priority_names[priority] << "] UnLocked." << endl;
    }
}

int main() {
    srand(time(0));
    cout << "Created lock." << endl;
    cout.flush();

    pthread_t t1, t2, t3, t4, t5, t6;
    pthread_attr_t pthread_custom_attr;
    pthread_attr_init(&pthread_custom_attr);

    pthread_create(&t1, 0, latch_test, (void*)5);    ++count_max;
    pthread_create(&t2, 0, latch_test, (void*)3);    ++count_max;
    pthread_create(&t3, 0, latch_test, (void*)4);    ++count_max;
    pthread_create(&t4, 0, latch_test, (void*)1);    ++count_max;
    pthread_create(&t5, 0, latch_test, (void*)2);    ++count_max;
    pthread_create(&t6, 0, latch_test, (void*)0);    ++count_max;

    while (count < count_max*iterations) {
        cout << (count/((float)count_max*iterations))*100.0 << "%" << endl;
        suspend(1000);
    }
    cout << "Done." << endl;

    return 0;
}
