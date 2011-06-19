// File: latch.h
// Written by Joshua Green

#ifndef LATCH_H
#define LATCH_H

#include "mutex.h"
#include <vector>
#include <map>

#ifndef WORDSIZE_64
    #pragma message("Compiling for 32bit.")
#else
    #pragma message("Compiling for 64bit.")
#endif

namespace LATCH_PRIORITIES {
    enum PRIORITY { LOWEST=0, LOW, MEDIUM, HIGH, HIGHEST };
}

class latch {
    private:
        volatile THREAD_ID _latch_owner;

        mutex _data_mutex;
        std::vector<THREAD_ID> _wait_queue;
        std::map<THREAD_ID, LATCH_PRIORITIES::PRIORITY> _wait_map;

        mutex _eval_mutex;
        volatile bool _cached_next_is_valid;
        THREAD_ID _cached_next;
        void _eval_next();
    public:
        latch();

        bool lock(const LATCH_PRIORITIES::PRIORITY=LATCH_PRIORITIES::MEDIUM, const bool wait=true);
        void unlock();
};


#endif // LATCH_H
