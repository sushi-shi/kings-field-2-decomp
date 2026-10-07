#ifndef KF_LIB_QUARTER_TURN_H
#define KF_LIB_QUARTER_TURN_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>

/* Quarter turns about Y applied by matrix_rotate_quarter_turns and
 * svector_rotate_quarter_turns. Map cells keep one in the low bits of their
 * quarter_turns byte and lighting rows hold one light matrix per turn. */
KF_ENUM_BEGIN(KfQuarterTurn, s32)
    KF_QUARTER_TURN_0 = 0,
    KF_QUARTER_TURN_1 = 1,
    KF_QUARTER_TURN_2 = 2,
    KF_QUARTER_TURN_3 = 3
KF_ENUM_END(KfQuarterTurn)

#endif
