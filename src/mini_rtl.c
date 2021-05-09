/*
 * This code offers simple versions of a few RTL functions,
 * for use when building with the EDISOFT toolchain.
 *
 * Thanassis Tsiodras
 * Real-time Embedded Software Engineer
 * System, Software and Technology Department
 * European Space Agency
 * e-mail:
 *    Thanassis.Tsiodras@esa.int (work) 
 *    ttsiodras@gmail.com (Personal)
 */


#ifdef EDISOFT

/*
 * We can't use any of EDISOFT's proprietary code; so this is a small "RTL"
 * that offers the necessary pieces. The cross-compiler DOES NOT come
 * with a library that we can bundle.
 */

#include "mini_rtl.h"

void *memset(void *p, int c, long unsigned int size)
{
    unsigned char *q = (unsigned char *)p;
    while(size--) {
        *q++ = c;
    }
    return p;
}

void *memcpy(void *dest, const void *src, long unsigned int size)
{
    unsigned char *p=dest;
    const unsigned char *q=src;
    while(size--)
        *p++ = *q++;
    return dest;
}

void *memdup(const void *p, unsigned size)
{
    unsigned char *q = malloc(size);
    memcpy(q, p, size);
    return q;
}

unsigned strlen(const char *q)
{
    unsigned len = 0;
    while (*q++)
        len++;
    return len;
}

char *strdup(const char *q)
{
    unsigned len = strlen(q);
    char *r = malloc(len)+1;
    while((*r++ = *q++));
    return r;
}

int strcmp(const char *s1, const char *s2)
{
    for(;*s1==*s2; s1++,s2++)
        if (*s1 == '\0')
            return 0;
    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

char pseudoHeap[16*1024*1024];
void *malloc(unsigned size)
{
    static unsigned offset = 0;
    static unsigned available = sizeof(pseudoHeap);
    if (size<available) {
        void *ret = &pseudoHeap[offset];
        offset += size;
        available -= size;
        memset(ret, 0, size);
        return ret;
    } else
        return NULL;
}

void free(const void *unused)
{
    (void)unused;
}

#endif
