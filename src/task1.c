#include "system.h"

#if !defined(EDISOFT)

#include <stdio.h>
#include <stdint.h>

#ifdef __linux__
#include <time.h>
#endif

#endif

uint64_t getNanos(void);

uint64_t getNanos(void)
{
#ifdef __rtems__

#if __RTEMS_MAJOR__ > 4 || \
    (__RTEMS_MAJOR__ == 4 && __RTEMS_MINOR__ > 10)
    return rtems_clock_get_uptime_nanoseconds();
#else
    return 0;
#endif

#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ts.tv_sec*1e9 + ts.tv_nsec;
#endif
}

void task1()
{
    int j = 0;
    double d = 1.0;
    uint64_t t1, t2;
#if defined(CHECK_FPU_EXCEPTIONS)
    unsigned oldfsr, newfsr;

    double bb = 1.79769313486231470e+308;
    double cc = bb/0.2;

    asm ("st %%fsr, %0" : "=m" (oldfsr));
    newfsr = oldfsr | (0x1E << 23);
    asm ("ld %0, %%fsr" :: "m" (newfsr));

    double ee = bb/0.2;
#endif

#if !defined(EDISOFT)
    printf("[TASK1] Starting...\n");
    fflush(stdout);
#endif

    t1 = getNanos();
    for(j=0; j<1000; j++) {
        int k;
        for(k=0; k<1000; k++)
            d+=1.001;
        d*=1.0001;
    }
    t2 = getNanos();

#if !defined(EDISOFT)
    printf("[TASK1] Loop completed in %.3f us.\n", ((double)(t2-t1))/1000.0);
#endif
    {
        long result = (long) d;
        long target = 1052811;
        if (target != result) {
#if !defined(EDISOFT)
            printf(
                "[TASK] Failed! Computed value was %ld instead of %ld\n",
                result, target);
            fflush(stdout);
#endif
            die(1);
        }
    }
#if !defined(EDISOFT)
    printf("[TASK1] Computed the correct floating point result.\n");
    fflush(stdout);
#endif
}
