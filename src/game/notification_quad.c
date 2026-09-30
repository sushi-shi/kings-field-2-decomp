#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/notification_quad.h>

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
