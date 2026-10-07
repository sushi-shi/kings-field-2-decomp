#ifndef KF_LIB_TYPES_H
#define KF_LIB_TYPES_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;

/* Old C word result that no path assigns: retail schedules the epilogue for
 * a live v0, while every caller discards it. C++ rejects a valueless return
 * from a non-void function, so the modern view checks it as void. */
#ifdef __cplusplus
#define KF_VALUELESS_S32 void
#else
#define KF_VALUELESS_S32 s32
#endif

#endif
