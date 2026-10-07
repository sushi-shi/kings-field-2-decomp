#ifndef KF_PSYQ_H
#define KF_PSYQ_H

/*
 * Single guarded entry point for the Psy-Q SDK headers, which ship without
 * include guards of their own. Include this instead of the raw SDK headers so
 * a TU pulls each in exactly once and gets the real library prototypes and
 * types rather than hand-rolled externs.
 *
 * LIBSND and LIBCD stay in psyq/audio.h so their declarations are limited
 * to audio/CD users. KERNEL and the BIOS file interfaces stay in
 * psyq/kernel.h, which also supplies the release's missing LIBAPI
 * declarations and case-sensitive include shim. C runtime declarations stay
 * in psyq/libc.h so only their callers include MEMORY.H and MALLOC.H.
 */
#include <sys/types.h>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBETC.H>

/* LIBETC exports VSync, but Psy-Q 3.0's LIBETC.H omits its declaration;
 * later releases declare it with this signature. */
extern int VSync(int mode);

/* REG.OBJ exports this helper, but Psy-Q 3.0's LIBGTE.H omits it. The second
 * pointer is inherited from the SLPS-00017 callers; review against KF2 calls. */
extern void ReadSZ2(long *depth, long *unused_depth);

#endif
