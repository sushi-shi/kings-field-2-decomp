#ifndef KF_PSX_LIBC_H
#define KF_PSX_LIBC_H

// C library and BIOS file calls as the original sources use them. The game
// translation units are freestanding; each name binds to a kf_psx_* runtime
// symbol so it never resolves to the host C library by accident.

#include <kf/psx/sdk.h>

#define KF_PSX_SYMBOL(name) __asm__("kf_psx_" #name)

#ifdef __cplusplus
extern "C" {
#endif

void *memcpy(void *destination, const void *source, unsigned long size) KF_PSX_SYMBOL(memcpy);
void *memset(void *destination, int value, unsigned long size) KF_PSX_SYMBOL(memset);
void *malloc(unsigned long size) KF_PSX_SYMBOL(malloc);
void free(void *allocation) KF_PSX_SYMBOL(free);
int rand(void) KF_PSX_SYMBOL(rand);
void srand(unsigned int seed) KF_PSX_SYMBOL(srand);
int printf(const char *format, ...) KF_PSX_SYMBOL(printf);
void exit(int status) KF_PSX_SYMBOL(exit);
char *strcpy(char *destination, const char *source) KF_PSX_SYMBOL(strcpy);
char *strcat(char *destination, const char *source) KF_PSX_SYMBOL(strcat);
int strncmp(const char *left, const char *right, unsigned long count) KF_PSX_SYMBOL(strncmp);
int abs(int value) KF_PSX_SYMBOL(abs);
int atoi(const char *string) KF_PSX_SYMBOL(atoi);

long open(const char *name, long mode) KF_PSX_SYMBOL(open);
long close(long file) KF_PSX_SYMBOL(close);
long lseek(long file, long offset, long origin) KF_PSX_SYMBOL(lseek);
long read(long file, void *buffer, long length) KF_PSX_SYMBOL(read);
long write(long file, const void *buffer, long length) KF_PSX_SYMBOL(write);
long erase(const char *name) KF_PSX_SYMBOL(erase);
long bios_delete(const char *name) KF_PSX_SYMBOL(delete);
long format(const char *device) KF_PSX_SYMBOL(format);
struct DIRENTRY *firstfile(const char *pattern, struct DIRENTRY *entry) KF_PSX_SYMBOL(firstfile);
struct DIRENTRY *nextfile(struct DIRENTRY *entry) KF_PSX_SYMBOL(nextfile);

#ifdef __cplusplus
}
#endif

#define FREAD 0x0001
#define FWRITE 0x0002
#define FNBLOCK 0x0004
#define FCREAT 0x0200
#define FTRUNC 0x0400
#define FASYNC 0x8000

#endif // KF_PSX_LIBC_H
