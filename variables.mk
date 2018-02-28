#########################################################################
# Create the build settings based on the configuration set in config.mk
# and passed at make's command line (e.g. make CFG=debug CPU=leon2 ...
#########################################################################

# Target folder suffix; every significant configuration option into account
SUFFIX=$(CFG).$(CPU).$(PLATFORM)

# Build a Dependency list and an Object list, by replacing the .c
# extension to .d for dependency files, and .o for object files.
DEP = $(patsubst %.c, deps.$(SUFFIX)/%.d, ${SRC})
OBJ = $(patsubst %.c, objs.$(SUFFIX)/%.o, ${SRC})

ifeq (${PLATFORM},LINUX)

CC = gcc
GCOV = gcov
STRIP = strip
OBJCOPY = objcopy
GDB = gdb
QEMU = qemu-system-$(shell uname -m)

else

CC = ${RTEMS}/bin/${CROSS_PREFIX}-gcc
GCOV = ${RTEMS}/bin/${CROSS_PREFIX}-gcov
STRIP = ${RTEMS}/bin/${CROSS_PREFIX}-strip
OBJCOPY = ${RTEMS}/bin/${CROSS_PREFIX}-objcopy
GDB = ${RTEMS}/bin/${CROSS_PREFIX}-gdb
QEMU = ./contrib/qemu-leon2/bin/qemu-system-sparc 

endif

# What compiler to use for generating dependencies: 
# it will be invoked with -MM -MP
CDEP = ${CC}

# What include flags to pass to the compiler
INCLUDEFLAGS += -I src -D${PLATFORM}

ifeq ($(COVERAGE),1)
CFLAGS += -DCOVERAGE_ENABLED -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

# Compiler flags
CFLAGS += -Wall -Wextra -g ${INCLUDEFLAGS} \
	  -Wmissing-prototypes -Wimplicit-function-declaration \
	  -Wstrict-prototypes -Wnested-externs

ifeq ($(CFG),debug)

CFLAGS += -DDEBUG
ifeq ($(COVERAGE),1)
CFLAGS+=-O0
endif

else # release build

CFLAGS += -O2 -DNDEBUG

ifeq ($(COVERAGE),1)
$(error You are not supposed to use coverage in release builds - read about it e.g. inlining issues, etc)
endif

endif

# RTEMS flags
ifneq (${PLATFORM},LINUX)
CFLAGS += -B${RTEMS_LIB} -specs bsp_specs -qrtems -mcpu=cypress -DBSP_${CPU} 
LDFLAGS += -B${RTEMS_LIB} -specs bsp_specs -qrtems -mcpu=cypress
endif

# Garbage collect unused functions at link-time
# CFLAGS += -ffunction-sections -fdata-sections
# LDFLAGS += -Wl,--gc-sections
