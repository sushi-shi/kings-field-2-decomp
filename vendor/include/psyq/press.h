#ifndef KF_PSYQ_PRESS_H
#define KF_PSYQ_PRESS_H
/* MDEC decoder SDK header, guarded (it ships without a guard). */
#include <psyq/sdk.h>
#include <LIBPRESS.H>

/* LIBETC's CALLBACK.OBJ exports this MDEC output hook; Psy-Q 3.0 LIBPRESS.H
 * omits it. The return value is unused by every caller. */
extern int DecDCToutCallback(void (*func)());

#endif
