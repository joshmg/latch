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

namespace LATCH {

    #define PRIORITIES_COUNT 5
    enum PRIORITY { LOWEST_PRIORITY=0, LOW_PRIORITY, MEDIUM_PRIORITY, HIGH_PRIORITY, HIGHEST_PRIORITY };

    class latch {
        private:
            volatile THREAD_ID _latch_owner;

            mutex _data_mutex;
            std::vector<THREAD_ID> _wait_queue;
            std::map<THREAD_ID, PRIORITY> _wait_map;

            mutex _eval_mutex;
            volatile bool _cached_next_is_valid;
            THREAD_ID _cached_next;
            int _priorities_waiting_count[PRIORITIES_COUNT];
            void _eval_next();
        public:
            latch();

            bool lock(const PRIORITY=MEDIUM_PRIORITY, const bool wait=true);
            void unlock();
    };

}

#endif // LATCH_H
