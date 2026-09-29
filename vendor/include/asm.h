#ifndef KF_ASM_SHIM
#define KF_ASM_SHIM
/* Case shim: Psy-Q 3.0 KERNEL.H includes <asm.h> (lower case), which does
   not resolve on a case-sensitive host. Forward to the real ASM.H. */
#include <ASM.H>
#endif
