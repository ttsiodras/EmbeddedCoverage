#ifndef __MINI_RTL_H__
#define __MINI_RTL_H__

#ifdef EDISOFT

#include <string.h>

#define exit(x) while(1);
#define NULL ((void *)0)

void *malloc(unsigned size);
void free(const void *unused);
char *strdup(const char *q);
unsigned strlen(const char *q);
int strcmp(const char *s1, const char *s2);
void *memdup(const void *p, unsigned size);

#else // not EDISOFT

#include <stdio.h>
#include <string.h>

#endif // EDISOFT

#endif // __MINI_RTL_H__
