#ifndef KF_PSYQ_CD_H
#define KF_PSYQ_CD_H

/* Guarded entry point for the unguarded Psy-Q 3.0 LIBCD.H. */

#include <psyq/sdk.h>
#include <LIBCD.H>

/* CDREAD.OBJ exports this status poll, omitted by Psy-Q 3.0 LIBCD.H. */
extern int CdReadSync(int mode, unsigned char *result);

#endif
