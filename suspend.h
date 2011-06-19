// File: suspend.h
// Written by Joshua Green

#ifndef SUSPEND_H
#define SUSPEND_H

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    #include <windows.h>
    #define suspend(x) (Sleep((x)))
#else
    #include <unistd.h>
    #define suspend(x) (usleep((x)*1000))
#endif 


#endif // SUSPEND_H
