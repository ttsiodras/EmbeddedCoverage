#ifdef __rtems__
#define CONFIGURE_INIT
#endif

#include "system.h"

#if !defined(EDISOFT)
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef COVERAGE_ENABLED
#include "gcov_public.h"
#endif

#ifdef __rtems__

#include "mini_rtl.h"

void die(int exitCode)
{
    (void)exitCode;
    rtems_task_delete(RTEMS_SELF);
}

rtems_task Init(rtems_task_argument argument)
{
    (void) argument;
    // Maybe disable interrupts here, to be sure the tests run uninterrupted
    // from any other tasks or HW triggers
    //
    // Initialize time getting function here
    my_main();
#ifdef COVERAGE_ENABLED
    __gcov_exit();
#endif
#if !defined(EDISOFT)
    puts("All done! :-) You can hit Ctrl-C now.");
#endif
    exit(0);
}

#else

#include <stdlib.h>

void die(int exitCode)
{
    exit(exitCode);
}

int main(void)
{
    return my_main();
}

#endif // __rtems__
