/*
 * This code was created based on information related to GCOV existing in
 * various places inside GCC and the Linux kernel.
 *
 * Its main entry point is the CreateGCDAinMemory function, that is called from
 * the __gcov_exit hook at the end of execution; this function will generate
 * the .gcda files information in a memory buffer, which we then dump to the
 * host filesystem via GDB scripting.
 *
 * Thanassis Tsiodras
 * Real-time Embedded Software Engineer
 * System, Software and Technology Department
 * European Space Agency
 * e-mail:
 *    Thanassis.Tsiodras@esa.int (work) 
 *    ttsiodras@gmail.com (Personal)
 */

#ifndef __GCOV_H__
#define __GCOV_H__

#include "gcov_public.h"

/*
 * Macros defining magic values and tag types that are used when the coverage
 * flags (-fprofile-arcs -ftest-coverage) are passed to GCC.
 */
#define GCOV_DATA_MAGIC		((unsigned int) 0x67636461)
#define GCOV_TAG_FUNCTION	((unsigned int) 0x01000000)
#define GCOV_TAG_FUNCTION_LENGTH	3
#define GCOV_TAG_COUNTER_BASE	((unsigned int) 0x01a10000)
#define GCOV_TAG_FOR_COUNTER(count)					\
	(GCOV_TAG_COUNTER_BASE + ((unsigned int) (count) << 17))

/* The gcov structures change between different GCC versions. We therefore use
 * an opaque structure, a pointer to which is passed to us during __gcov_init.
 * We store these pointers in a linked list, and iterate over them at
 * __gcov_exit time - re-creating the complete coverage info in GCDA format. */
struct gcov_info;

/* Since the structure is opaque, we delegate getting data out of it to helper
 * functions - different per coverage version. */
const char *gcov_info_filename(struct gcov_info *info);
unsigned CreateGCDAinMemory(char *buffer, struct gcov_info *info);

#endif /* __GCOV_H__ */
