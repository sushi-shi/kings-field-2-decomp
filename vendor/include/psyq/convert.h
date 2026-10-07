#ifndef KF_PSYQ_CONVERT_H
#define KF_PSYQ_CONVERT_H

/* Guarded entry point for the unguarded Psy-Q 3.0 CONVERT.H. */
#include <CONVERT.H>

/* CONVERT.H declares atoi without parameters, which C++ reads as (void). */
#if defined(__cplusplus)
extern int atoi(const char *string);
#endif

#endif
