#ifndef KF_GAME_EVENT_COUNTER_H
#define KF_GAME_EVENT_COUNTER_H

#include <kf/lib/bool.h>
#include <kf/lib/types.h>
#include <kf/game/item.h>

extern u8 game_counter_bytes[KF_ITEM_ID_COUNT];

b32 game_counter_decrement(KfObjectId index);
b32 game_counter_increment(KfObjectId index);

#endif
