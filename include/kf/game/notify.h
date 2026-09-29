#ifndef KF_GAME_NOTIFY_H
#define KF_GAME_NOTIFY_H

#include <kf/lib/types.h>

/* Message ids for the GAME.EXE notification queue. KF2 inserts one message
 * before the power-increase pair (King's Field used 30 and 31). */
enum {
    KF_NOTIFICATION_LEVEL_UP = 0,
    KF_NOTIFICATION_PHYSICAL_POWER_INCREASED = 31,
    KF_NOTIFICATION_MAGIC_POWER_INCREASED = 32
};

void notify_enqueue(s32 message_id, ...);

#endif
