#include "atomic.h"

#include <stdio.h>

void atomic_test()
{
    int a = 0;
    int *ptr = &a;

    printf("a=%d\r\n", a);
    a_store(ptr, 100);
    printf("a=%d\r\n", a);

    int old = a_ll(ptr);
    asm volatile
    (
        "mov r0, #12\n"
        "str r0, [%0]"
        :
        :"p"(ptr)
        :"r0"
    );
    // printf("a=%d\r\n", a);
    int str_ret = a_sc(ptr, 50);
    printf("a=%d, str_ret=%d, old=%d\r\n", a, str_ret, old);
}