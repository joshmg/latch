// File: mutex.h
// Written by Joshua Green

#ifndef MUTEX_H
#define MUTEX_H

#ifndef WORDSIZE_64
    #pragma message("Compiling for 32bit.")
#else
    #pragma message("Compiling for 64bit.")
#endif

#ifdef __GNUC__
    #include <pthread.h>
    typedef pthread_t THREAD_ID;
#else
    #include <process.h>
    #ifdef WORDSIZE_64
        #if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
            #include <windows.h>
            typedef LONGLONG THREAD_ID;
        #else
            typedef unsigned long long THREAD_ID;
        #endif
    #else
        #if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
            #include <windows.h>
            typedef DWORD THREAD_ID;
        #else
            typedef unsigned long THREAD_ID;
        #endif
    #endif
#endif


class mutex {
    private:
        volatile THREAD_ID _mutex_owner;
    public:
        mutex();

        bool lock(const bool wait=true);
        void unlock();
};

#endif // MUTEX_H
