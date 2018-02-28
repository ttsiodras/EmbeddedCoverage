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


#ifndef __GNUC__
#error All the coverage functionality depends on the inside workings of GCC. Aborting...
#endif

/* Two common APIs used for both old and new versions of GCC, to build
 * up the data of the GCDA files: one stores 32 bit values, the other
 * 64 bit values (the line and branch counts).

 * store_gcov_u32 - used during GCDA creation to store a 32bit value
 *
 * @buffer : target buffer or NULL
 * @off    : offset into the buffer
 * @v      : value to be stored
 *
 * Returns the number of bytes stored.
 * If @buffer is NULL, doesn't store anything (used to calculate buffer size)
 */
static unsigned store_gcov_u32(void *buffer, unsigned off, unsigned int v)
{
    unsigned int *data;

    if (buffer) {
        data = buffer + off;
        *data = v;
    }

    return sizeof(*data);
}

/**
 * store_gcov_u64 - used during GCDA creation to store a 64bit value
 *
 * @buffer : target buffer or NULL
 * @off    : offset into the buffer
 * @v      : value to be stored
 *
 * Returns the number of bytes stored.
 * If @buffer is NULL, doesn't store anything (used to calculate buffer size)
 */
static unsigned store_gcov_u64(
    void *buffer, unsigned off, unsigned long long v)
{
    unsigned int *data;

    if (buffer) {
        data = buffer + off;

        data[0] = (v & 0xffffffffUL);
        data[1] = (v >> 32);
    }

    return sizeof(*data) * 2;
}

#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7))

#include "gcc_4_7.c"

#else

#include "gcc_3_4.c"

#endif
