#ifndef KF_LIB_TYPES_H
#define KF_LIB_TYPES_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;

#define KF_COUNTOF(array) ((s32)(sizeof(array) / sizeof((array)[0])))

#ifdef __cplusplus
#define KF_VALUELESS_S32 void
#else
#define KF_VALUELESS_S32 s32
#endif

#endif
