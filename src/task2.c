#include "system.h"

#if !defined(EDISOFT)
#include <stdio.h>
#endif

void task2()
{
    int j = 0;
    long long int dummy = 0;

#if !defined(EDISOFT)
    printf("[TASK2] Starting...\n");
    fflush(stdout);
#endif
    for(j=0; j<500000; j++) {
        int k;
        for(k=0; k<100; k++)
            dummy ^= k;
        dummy++;
    }
    if (dummy != 500000) {
#if !defined(EDISOFT)
        printf(
            "[TASK2] Failed! Computed value was %lld instead of 500000\n",
            dummy);
        fflush(stdout);
#endif
        die(1);
    }
#if !defined(EDISOFT)
    printf("[TASK2] Computed the correct integer result.\n");
    fflush(stdout);
#endif
}
