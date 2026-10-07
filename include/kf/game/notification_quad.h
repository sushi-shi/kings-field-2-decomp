#ifndef KF_GAME_NOTIFICATION_QUAD_H
#define KF_GAME_NOTIFICATION_QUAD_H

#include <kf/lib/offsetof.h>
#include <kf/lib/types.h>
#include <kf/game/render_types.h>

typedef struct KfNotificationQuad {
    KfSpriteState kind;
    u8 texture_u;
    u8 texture_v;
    u8 texture_width;
    u8 texture_height;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 clut;
    u16 tpage;
} KfNotificationQuad;

typedef char kf_notification_quad_size[sizeof(KfNotificationQuad) == 18 ? 1 : -1];
typedef char kf_notification_quad_x_offset[
    offsetof(KfNotificationQuad, x) == 6 ? 1 : -1];

extern KfNotificationQuad notification_quads[7];

void notification_draw_quad(const KfNotificationQuad *source, u16 tpage_flags, const u8 *color);
void notification_draw(void);

#endif
