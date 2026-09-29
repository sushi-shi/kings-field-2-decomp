#ifndef KF_R3000_SHIM
#define KF_R3000_SHIM
/* Case shim: Psy-Q 3.0 KERNEL.H includes <r3000.h> (lower case), which does
   not resolve on a case-sensitive host. Forward to the real R3000.H. */
#include <R3000.H>
#endif
