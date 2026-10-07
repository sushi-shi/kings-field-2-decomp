extern "C" {
#ifndef KF_PSYQ_H
#define KF_PSYQ_H

#include <sys/types.h>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBETC.H>

extern int VSync(int mode);

extern void ReadSZ2(long *depth, long *unused_depth);

#endif

}
