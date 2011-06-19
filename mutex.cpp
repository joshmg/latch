// File: mutex.cpp
// Written by Joshua Green

#include "mutex.h"
#include "suspend.h"
#include "atomic_if_then_assign.h"

#ifdef __GNUC__
    #include <pthread.h>
    #define get_thread_id pthread_self
#else
    #include <process.h>
    #define get_thread_id GetCurrentThreadId
#endif


mutex::mutex() : _mutex_owner(0) {}

bool mutex::lock(const bool wait) {
    const THREAD_ID lock_id = get_thread_id();
    
    while(atomic_if_then_assign(&_mutex_owner, lock_id, 0) != 0) {
        if (wait) suspend(0);
        else return false;
    }
    return true;
}

void mutex::unlock() {
    _mutex_owner = 0;
}
