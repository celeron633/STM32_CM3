#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <stdint.h>

void atomic_test();

static inline void a_barrier()
{
    asm volatile
    (
        "dmb ish\n"
        :
        :
        :"memory"
    );
}

static inline int a_ll(volatile int *p)
{
    int v = 0;
    asm volatile
    (
        "ldrex %0, [%1]\n"
        :"=r"(v)
        :"p"(p)
        :
    );

    return v;
}

static inline int a_sc(volatile int *p, int v)
{
    int r = 0;
    asm volatile
    (
        "strex %0, %2, [%1]\n"
        :"=r"(r)
        :"p"(p), "r"(v)
        :
    );

    return !r;
}

static inline void a_store(volatile int *p, int v)
{
    a_barrier();
    *p = v;
    a_barrier();
} 

#endif