ifeq (${PLATFORM},EDISOFT)

RTEMS ?= /opt/rtems-4.8
RTEMS_MAKEFILE_PATH = ${RTEMS}/sparc-rtems4.8/${CPU}
RTEMS_LIB=${RTEMS_MAKEFILE_PATH}/lib
CROSS_PREFIX=sparc-rtems4.8

SRC += mini_rtl.c  # Add a portable mini-RTL for EDISOFT

CFLAGS += -nodefaultlibs -DEDISOFT
LDFLAGS += -nodefaultlibs                                                 \
           ${RTEMS_LIB}/libno_event.a ${RTEMS_LIB}/libno_msg.a            \
           ${RTEMS_LIB}/libno_sem.a ${RTEMS_LIB}/libno_timer.a            \
           ${RTEMS_LIB}/libno_rtmon.a ${RTEMS_LIB}/libno_ext.a            \
           -Wl,--start-group -lrtemscpu -lio -lno_event -lmsg -lno_sem    \
           -lno_timer -lno_rtmon -lno_ext -lrtemsbsp -lrtemscpu           \
           -Wl,--end-group -Wl,--script=linkcmds -lm -lgcc

else ifeq (${PLATFORM},OAR)

RTEMS?=/opt/rtems-4.12
CROSS_PREFIX?=sparc-rtems4.12
RTEMS_MAKEFILE_PATH = ${RTEMS}/${CROSS_PREFIX}/${CPU}
RTEMS_LIB=${RTEMS_MAKEFILE_PATH}/lib

else ifeq (${PLATFORM},GAISLER)

RTEMS?=/opt/rtems-4.10-gaisler-1.2.20-gcc.4.4.6
CROSS_PREFIX?=sparc-rtems
RTEMS_MAKEFILE_PATH = ${RTEMS}/${CROSS_PREFIX}/${CPU}
RTEMS_LIB=${RTEMS_MAKEFILE_PATH}/lib

endif
