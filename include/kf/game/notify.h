#ifndef KF_GAME_NOTIFY_H
#define KF_GAME_NOTIFY_H

#include <kf/lib/types.h>
#include <kf/game/notification_quad.h>
#include <kf/game/notify_types.h>

enum {
    KF_NOTIFICATION_TEXT_SPRITE = 0,
    KF_NOTIFICATION_GOLD_SPRITE = 1,
    KF_NOTIFICATION_ONES_SPRITE = 2,
    KF_NOTIFICATION_TENS_SPRITE = 3,
    KF_NOTIFICATION_HUNDREDS_SPRITE = 4,
    KF_NOTIFICATION_THOUSANDS_SPRITE = 5,
    KF_NOTIFICATION_SPRITE_COUNT = 6
};

typedef KfNotificationQuad KfNotificationDigitSprite;

typedef char kf_notification_digit_sprite_size[sizeof(KfNotificationDigitSprite) == 18 ? 1 : -1];

void notify_enqueue(KfNotificationId message_id, ...);
void notification_digit_set_v(KfNotificationDigitSprite *sprite, s32 digit);
void notification_update(void);

#endif
