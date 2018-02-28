#ifndef __GCOV_PUBLIC_H__
#define __GCOV_PUBLIC_H__

// Opaque - different per GCC version
struct gcov_info;

// Basically, the type of the counter for each branch/line
// Always 64bit so far.
typedef long long gcov_type;

// Called at startup - and at least once for every file with coverage info
void __gcov_init(struct gcov_info *info);

// Called once at the end - at which point we iterate over all the 
// gcov_info that we collected during __gcov_init and call CreateGCDAinMemory
// for each one of them - dumping the memory via GDB.
void __gcov_exit(void);

// Never called - but must be there for GCOV link semantics to work
void __gcov_merge_add(gcov_type *counters, unsigned int n_counters);

// The workhorse - different for [GCC3.4, 4.7) vs [4.7, ...)
unsigned int CreateGCDAinMemory(char *buffer, struct gcov_info *info);

#endif
