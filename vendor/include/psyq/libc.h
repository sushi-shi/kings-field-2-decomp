#ifndef KF_PSYQ_LIBC_H
#define KF_PSYQ_LIBC_H

/*
 * Psy-Q 3.0 C runtime interfaces. MEMORY.H and MALLOC.H preserve the
 * declarations supplied with the pinned SDK. RAND.H supplies RAND_MAX but no
 * callable declaration; the remaining LIBAPI entry points used by the game
 * are also absent from this release's headers.
 */
#include <MEMORY.H>
#include <MALLOC.H>
#include <RAND.H>

extern int rand(void);
extern int printf(const char *format, ...);
extern void exit(int status);
extern char *strcpy(char *destination, const char *source);
extern char *strcat(char *destination, const char *source);
extern int strncmp(const char *left, const char *right, unsigned long count);
/* LIBC exports abs, but Psy-Q 3.0 comments its CONVERT.H declaration out and
 * ABS.H's macro is unparenthesized. GCC expands this prototype as its builtin. */
extern int abs(int value);

#if defined(__cplusplus)
extern void *memcpy(void *destination, const void *source, unsigned long size);
extern void *memset(void *destination, int value, unsigned long size);
extern void *malloc(unsigned long size);
extern void free(void *allocation);
#endif

#endif
