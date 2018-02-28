/*
 * This code was created based on information related to GCOV existing in
 * various places inside both GCC and the Linux kernel.
 *
 * Its main point is the CreateGCDAinMemory function; called from the
 * __gcov_exit hook at the end of execution. This function will generate
 * the .gcda files information in a memory buffer, which we then dump
 * to the host filesystem via GDB scripting.
 *
 * For information about the inner workings of this code, please study
 * GCC's gcov-io.{h,c} and then look at my post at:
 *
 *     https://www.thanassis.space/coverage.html
 *
 * Thanassis Tsiodras
 * Real-time Embedded Software Engineer
 * System, Software and Technology Department
 * European Space Agency
 * e-mail:
 *    Thanassis.Tsiodras@esa.int (work) 
 *    ttsiodras@gmail.com (Personal)
 */


#include "gcov_public.h"
#include "gcov_internal.h"

#ifdef EDISOFT

#include "mini_rtl.h"

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif

typedef struct tagGcovInfo {
    struct gcov_info *info;
    struct tagGcovInfo *next;
} GcovInfo;
GcovInfo *headGcov = NULL;

/*
 * __gcov_init is called by gcc-generated constructor code for each object
 * file compiled with -fprofile-arcs.
 */
void __gcov_init(struct gcov_info *info)
{
    int metBefore = 0;
    GcovInfo *tmp = headGcov;
    while(tmp) {
        if (!strcmp(gcov_info_filename(tmp->info), gcov_info_filename(info))) {
            metBefore = 1;
            break;
        }
        tmp = tmp->next;
    }

    if (!metBefore) {
        //printf("__gcov_init called for %s!\n", gcov_info_filename(info));
        //fflush(stdout);
    }

    GcovInfo *newHead = malloc(sizeof(GcovInfo));
    if (!newHead) {
        //puts("Out of memory!");
        //fflush(stdout);
        exit(1);
    }
    newHead->info = info;
    newHead->next = headGcov;
    headGcov = newHead;
}

void __gcov_exit()
{
    GcovInfo *tmp = headGcov;
    while(tmp) {
        char *buffer;
        unsigned bytesNeeded = CreateGCDAinMemory(NULL, tmp->info);
        buffer = malloc(bytesNeeded);
        if (!buffer) {
            //puts("Out of memory!");
            //fflush(stdout);
            exit(1);
        }
        CreateGCDAinMemory(buffer, tmp->info);
        //printf("Emitting %6d bytes for %s\n", bytesNeeded, gcov_info_filename(tmp->info));
        //fflush(stdout);
        free(buffer);
        tmp = tmp->next;
    }
}

void __gcov_merge_add(gcov_type *counters, unsigned int n_counters)
{
    (void) counters;
    (void) n_counters;
    //puts("__gcov_merge_add isn't called, right? Right? RIGHT?");
    //fflush(stdout);
    exit(1);
}
