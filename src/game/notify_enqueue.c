#include <stdarg.h>

#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>
#include <kf/game/notification_quad.h>
#include <kf/game/notify.h>

ADDRESS(0x80032fec, 0x154)
void notification_draw_quad(const KfNotificationQuad *source, u16 tpage_flags, const u8 *color)
{
    POLY_FT4 *quad = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;

    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
        game_graphics_runtime.display_state.primitive_buffer->end) {
        return;
    }

    setPolyFT4(quad);
    setSemiTrans(quad, 1);
    quad->x0 = quad->x2 = source->x;
    quad->x1 = quad->x3 = source->x + source->width;
    quad->y0 = quad->y1 = source->y;
    quad->y2 = quad->y3 = source->y + source->height;
    quad->clut = source->clut;
    quad->tpage = source->tpage | tpage_flags;
    quad->u0 = quad->u2 = source->texture_u;
    quad->u1 = quad->u3 = source->texture_u + source->texture_width;
    quad->v0 = quad->v1 = source->texture_v;
    quad->v2 = quad->v3 = source->texture_v + source->texture_height;
    setRGB0(quad, color[0], color[1], color[2]);
    AddPrim(game_graphics_runtime.display_state.ordering_table + 1, quad);
}

ADDRESS(0x80033140, 0x90)
void notification_draw(void)
{
    KfNotificationQuad *quad = notification_quads;
    u8 color[3];

    color[0] = color[1] = color[2] = game_graphics_runtime.notification_brightness;
    if (quad->kind == 0xff) {
        return;
    }
    do {
        if (quad->kind != 0) {
            notification_draw_quad(quad, 0x20, color);
            notification_draw_quad(quad, 0x40, color);
        }
        quad++;
    } while (quad->kind != 0xff);
}

DATA(0x80066808, 0x7e)
KfNotificationQuad notification_quads[7] = {
    {0, 0, 0, 127, 14, 0, 96, 203, 127, 14, 0x7f24, 0x1b},
    {0, 0, 0, 127, 14, 0, 110, 203, 127, 14, 0x7f24, 0x1b},
    {0, 240, 0, 7, 14, 0, 90, 203, 7, 13, 0x7f64, 0x1d},
    {0, 240, 0, 7, 14, 0, 80, 203, 7, 13, 0x7f64, 0x1d},
    {0, 240, 0, 7, 14, 0, 70, 203, 7, 13, 0x7f64, 0x1d},
    {0, 240, 0, 7, 14, 0, 60, 203, 7, 13, 0x7f64, 0x1d},
    {0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

ADDRESS(0x800331d0, 0xa4)
void notify_enqueue(s32 message_id, ...)
{
    u8 *head;

    if (message_id > KF_NOTIFICATION_MAX_QUEUED_ID) {
        return;
    }
    head = &game_graphics_runtime.notification_control.queue_head;
    if (game_graphics_runtime.notification_message_ids[*head] == KF_NOTIFICATION_EMPTY) {
        game_graphics_runtime.notification_message_ids[*head] = message_id;
        if (message_id == KF_NOTIFICATION_PAYLOAD_ID) {
            va_list arguments;
            va_start(arguments, message_id);
            game_graphics_runtime.notification_payloads[*head] = va_arg(arguments, s32);
            va_end(arguments);
        }
        *head = (*head + 1) & (KF_NOTIFICATION_CAPACITY - 1);
    }
}

ADDRESS(0x80033274, 0x10)
void notification_digit_set_v(KfNotificationDigitSprite *sprite, s32 digit)
{
    sprite->texture_v = digit * 15;
}

static inline void notification_dequeue_group(void)
{
    KfNotificationControl *control;
    s32 id;

    control = &game_graphics_runtime.notification_control;
    id = game_graphics_runtime.notification_message_ids[
        game_graphics_runtime.notification_control.queue_tail];
    do {
        game_graphics_runtime.notification_message_ids[control->queue_tail] =
            KF_NOTIFICATION_EMPTY;
        control->queue_tail = (control->queue_tail + 1) & (KF_NOTIFICATION_CAPACITY - 1);
    } while (id == game_graphics_runtime.notification_message_ids[control->queue_tail]
             && id != KF_NOTIFICATION_PAYLOAD_ID);
    control->effect_phase = 0;
}

ADDRESS(0x80033284, 0x300)
void notification_update(void)
{
    u8 *phase = &game_graphics_runtime.notification_control.effect_phase;

    switch (*phase) {
    case 0: {
        u8 tail = game_graphics_runtime.notification_control.queue_tail;
        u8 id = game_graphics_runtime.notification_message_ids[tail];
        if (id == KF_NOTIFICATION_EMPTY) {
            break;
        }
        *phase = 1;
        game_graphics_runtime.notification_brightness = 0;
        game_graphics_runtime.notification_control.hold_frames = 15;
        if (id == KF_NOTIFICATION_PAYLOAD_ID) {
            s16 digits[12];

            notification_quads[0].kind = 0;
            notification_quads[1].kind = 1;
            notification_quads[1].texture_u = 128;
            notification_quads[1].texture_v = 42;
            menu_format_number(game_graphics_runtime.notification_payloads[tail],
                               4, 0, 0, digits);
            notification_quads[2].kind = 1;
            notification_digit_set_v(&notification_quads[2], (u16)digits[3]);
            notification_quads[3].kind = 1;
            notification_digit_set_v(&notification_quads[3], (u16)digits[2]);
            notification_quads[4].kind = 1;
            notification_digit_set_v(&notification_quads[4], (u16)digits[1]);
            notification_quads[5].kind = 1;
            notification_digit_set_v(&notification_quads[5], (u16)digits[0]);
        } else {
            notification_quads[0].kind = 1;
            notification_quads[5].kind = 0;
            notification_quads[4].kind = 0;
            notification_quads[3].kind = 0;
            notification_quads[2].kind = 0;
            notification_quads[1].kind = 0;
            notification_quads[0].texture_u = (id / 18) << 7;
            notification_quads[0].texture_v = (id % 18) * 14;
        }
        break;
    }
    case 1:
        game_graphics_runtime.notification_brightness += 20;
        if (game_graphics_runtime.notification_brightness >= 100) {
            *phase = 2;
        }
        break;
    case 2:
    {
        u8 frames = game_graphics_runtime.notification_control.hold_frames - 1;
        game_graphics_runtime.notification_control.hold_frames = frames;
        if (frames == 0) {
            *phase = 3;
        }
        break;
    }
    case 3:
        game_graphics_runtime.notification_brightness -= 20;
        if (game_graphics_runtime.notification_brightness == 0) {
            notification_quads[5].kind = 0;
            notification_quads[4].kind = 0;
            notification_quads[3].kind = 0;
            notification_quads[2].kind = 0;
            notification_quads[1].kind = 0;
            notification_quads[0].kind = 0;
            notification_dequeue_group();
        }
        break;
    }
}
