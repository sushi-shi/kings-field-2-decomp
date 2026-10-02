#ifndef KF_GAME_NOTIFY_H
#define KF_GAME_NOTIFY_H

#include <kf/lib/types.h>
#include <kf/game/notification_quad.h>

/* Message ids for the GAME.EXE notification queue. KF2 inserts one message
 * before the power-increase pair (King's Field used 30 and 31). */
enum {
    KF_NOTIFICATION_LEVEL_UP = 0,
    KF_NOTIFICATION_PHYSICAL_POWER_INCREASED = 31,
    KF_NOTIFICATION_MAGIC_POWER_INCREASED = 32,
    KF_NOTIFICATION_PAYLOAD_ID = 0x15,
    KF_NOTIFICATION_EMPTY = 0xff,
    KF_NOTIFICATION_MAX_QUEUED_ID = 239
};

/* Digit helpers operate on the same complete notification descriptor rows. */
typedef KfNotificationQuad KfNotificationDigitSprite;

typedef char kf_notification_digit_sprite_size[sizeof(KfNotificationDigitSprite) == 18 ? 1 : -1];

void notify_enqueue(s32 message_id, ...);
void notification_digit_set_v(KfNotificationDigitSprite *sprite, s32 digit);
void notification_update(void);

#endif
