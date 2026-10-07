#ifndef KF_OFFSETOF_H
#define KF_OFFSETOF_H

#include <kf/lib/null.h>
#include <kf/lib/types.h>

/* ANSI C's offsetof, which Psy-Q 3.0 STDDEF.H omits. The pinned GCC folds a
 * member address of the null object; C++ needs the builtin constant form. */
#ifndef offsetof
#ifdef __cplusplus
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((u32)&((type *)NULL)->member)
#endif
#endif

#endif
