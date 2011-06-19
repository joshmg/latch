// File: latch.cpp
// Written by Joshua Green

#include "latch.h"
#include "mutex.h"
#include "suspend.h"
#include "atomic_if_then_assign.h"
#include <vector>
#include <map>
using namespace std;
using namespace LATCH_PRIORITIES;

#ifdef __GNUC__
    #include <pthread.h>
    #define get_thread_id pthread_self
#else
    #include <process.h>
    #define get_thread_id GetCurrentThreadId
#endif

void latch::_eval_next() {
    if (!_cached_next_is_valid) {
        if (!_eval_mutex.lock(false)) { // another thread is performing the eval: there is no need to do it
            _data_mutex.lock();

            vector<THREAD_ID>::const_iterator it = _wait_queue.begin();
            vector<THREAD_ID>::const_iterator end = _wait_queue.end();
            PRIORITY lowest_priority = LOWEST;
            THREAD_ID next_thread = 0;
            if (it != end) next_thread = *it;

            while (it != end) {
                PRIORITY it_priority = (_wait_map.find(*it))->second;
                if (it_priority > lowest_priority) {
                    lowest_priority = it_priority;
                    next_thread = *it;
                }
                if (lowest_priority == HIGHEST) break;
                it++;
            }

            _cached_next = next_thread;
            _cached_next_is_valid = true;
            _data_mutex.unlock();
            _eval_mutex.unlock();
        }
    }

    if (atomic_if_then_assign(&_latch_owner, _cached_next, 0) == 0) {
        // Remove _cached_next from the pending queue since it has been given ownership
        _eval_mutex.lock();
        _data_mutex.lock();
        vector<THREAD_ID>::iterator it = _wait_queue.begin();
        vector<THREAD_ID>::const_iterator end = _wait_queue.end();
        while (it != end) {
            if (*it == _cached_next) {
                _wait_map.erase(_wait_map.find(*it));
                _wait_queue.erase(it);
                break;
            }
            it++;
        }
        _cached_next_is_valid = false;
        _data_mutex.unlock();
        _eval_mutex.unlock();
    }
}

latch::latch() : _latch_owner(0), _cached_next_is_valid(false), _cached_next(0) {}

bool latch::lock(const PRIORITY priority, const bool wait) {
    const THREAD_ID lock_id = get_thread_id();

    atomic_if_then_assign(&_latch_owner, lock_id, 0);
    if (_latch_owner == lock_id) return true;
    else if (!wait) return false;

    _data_mutex.lock();
    _wait_queue.push_back(lock_id);
    _wait_map[lock_id] = priority;
    _cached_next_is_valid = false;
    _data_mutex.unlock();

    while(_latch_owner != lock_id) {
        _eval_next();
        suspend(0);
    }

    return true;
}

void latch::unlock() {
    _latch_owner = 0;
}