#########################################################################
# Configuration section
#
# Use environment variables if found, otherwise fallback to sane defaults
#########################################################################

# If not explicitely selected, compile debug version of the binary
CFG?=debug

ifeq (${PLATFORM},LINUX)
CPU=$(shell uname -m)
else
# Default to leon2 (Euclid)
CPU ?= leon2
endif

# Your final binary's name
TARGET=fputest

# The directories containing the source files, separated by ':'
VPATH=src

# Your source files: regardless of where they reside in the source tree,
# VPATH will locate them...
SRC = init.c my_main.c task1.c task2.c

ifeq ($(COVERAGE),1)
ifneq (${PLATFORM},LINUX)
SRC += gcc.c gcov_hooks.c
endif
endif
