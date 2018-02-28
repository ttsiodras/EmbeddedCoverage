#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __rtems__

#include <rtems/system.h>
#include <rtems/score/types.h>
#include <rtems/rtems/types.h>
#include <rtems/rtems/tasks.h>

rtems_task Init(rtems_task_argument argument);

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS           64
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES  20
#define CONFIGURE_TICKS_PER_TIMESLICE     100
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#if !defined(EDISOFT)
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#endif

#include <rtems/confdefs.h>

#endif // __rtems__

// Prototypes for things called by my_main
extern void task1(void);
extern void task2(void);
extern void die(int exitCode);
extern int my_main(void);

#endif
