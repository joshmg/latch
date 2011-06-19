// File: atomic_if_then_assign.h
// Written by Joshua Green

#ifndef ATOMIC_IF_THEN_ASSIGN_H
#define ATOMIC_IF_THEN_ASSIGN_H

#ifdef __GNUC__

    // If GCC >= 4.1.2
    #if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 1 || (__GNUC_MINOR__ == 1 && __GNUC_PATCHLEVEL__ > 1)))
        #define atomic_if_then_assign(d, x, c) (__sync_val_compare_and_swap((d), (c), (x)))
    #else
        #define atomic_if_then_assign(d, x, c) 1
        #error "Requires GCC Version >= 4.1.2"
    #endif
#else
    #ifdef WORDSIZE_64
        #define atomic_if_then_assign(d, x, c) (InterlockedCompareExchange64((d), (x), (c)))
    #else
        #define atomic_if_then_assign(d, x, c) (InterlockedCompareExchange((d), (x), (c)))
    #endif
#endif 


#endif // ATOMIC_IF_THEN_ASSIGN_H
