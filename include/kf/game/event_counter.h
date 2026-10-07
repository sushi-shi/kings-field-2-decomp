#ifndef KF_GAME_EVENT_COUNTER_H
#define KF_GAME_EVENT_COUNTER_H

#include <kf/lib/bool.h>
#include <kf/lib/types.h>
#include <kf/game/item.h>

/* Event reset clears 0x1e words at this base. The menu reads byte +0x60. */
extern u8 game_counter_bytes[0x78];

b32 game_counter_decrement(KF_ENUM_PARAM(KfObjectId, s32) index);
b32 game_counter_increment(KF_ENUM_PARAM(KfObjectId, s32) index);

#endif
