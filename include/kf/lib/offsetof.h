#ifndef KF_OFFSETOF_H
#define KF_OFFSETOF_H

#include <kf/lib/null.h>
#include <kf/lib/types.h>

#ifndef offsetof
#ifdef __cplusplus
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((u32)&((type *)NULL)->member)
#endif
#endif

#endif
