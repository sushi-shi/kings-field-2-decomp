#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>

ADDRESS(0x800311b0, 0x144)
void render_textured_quad(s32 x, s32 y, s32 right, s32 bottom,
                   u8 texture_u, u8 texture_v, u8 texture_width,
                   u8 texture_height, u8 semitrans, u16 tpage,
                   u16 clut, u8 red, u8 green, u8 blue, s32 depth)
{
    POLY_FT4 *quad = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;

    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
        game_graphics_runtime.display_state.primitive_buffer->end) {
        return;
    }

    setPolyFT4(quad);
    if (semitrans != 0xff && semitrans != 0) {
        setSemiTrans(quad, 1);
    }
    setRGB0(quad, red, green, blue);
    quad->tpage = tpage;
    quad->clut = clut;
    quad->u0 = texture_u;
    quad->v0 = texture_v;
    quad->u1 = texture_u + texture_width;
    quad->v1 = texture_v;
    quad->u2 = texture_u;
    quad->v2 = texture_v + texture_height;
    quad->u3 = texture_u + texture_width;
    quad->v3 = texture_v + texture_height;
    quad->x0 = quad->x2 = x;
    quad->x1 = quad->x3 = right;
    quad->y0 = quad->y1 = y;
    quad->y2 = quad->y3 = bottom;

    if (depth > 0 && (u32)depth < KF_GAME_ORDERING_TABLE_LENGTH) {
        AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], quad);
    }
}

ADDRESS(0x800312f4, 0x90)
void render_sliding_panel_primary(void)
{
    s32 y = player_state.collision_lower_clearance;

    if (y < 240) {
        y += 120;
        if (y < 0) {
            y = 0;
        }
        render_textured_quad(0, y, 320, 240, 128, 192, 15, 15, 1, 55,
                             0x7bdc, 60, 60, 60, 64);
    }
}

ADDRESS(0x80031384, 0x90)
void render_sliding_panel_secondary(void)
{
    s32 y = player_state.collision_upper_clearance;

    if (y < 240) {
        y += 120;
        if (y < 0) {
            y = 0;
        }
        render_textured_quad(0, y, 320, 240, 144, 192, 15, 15, 1, 55,
                             0x7bdc, 60, 60, 60, 64);
    }
}

ADDRESS(0x80031414, 0xc0)
void render_color_overlay(void)
{
    if (game_graphics_runtime.color_overlay_control != 0xff) {
        render_textured_quad(0, 0, 0x140, 0xf0,
                             0x80, 0xd0, 0xf, 0xf, 1,
                             ((game_graphics_runtime.color_overlay_control & 3) << 5) | 0x17,
                             0x7bdc,
                             game_graphics_runtime.color_overlay_rgb[0],
                             game_graphics_runtime.color_overlay_rgb[1],
                             game_graphics_runtime.color_overlay_rgb[2],
                             (game_graphics_runtime.color_overlay_control & 0x80) ? 1 : 0x40);
    }
}

ADDRESS(0x800314d4, 0x28)
void render_set_color_overlay(u8 control, u8 red, u8 green, u8 blue)
{
    game_graphics_runtime.color_overlay_control = control;
    game_graphics_runtime.color_overlay_rgb[0] = red;
    game_graphics_runtime.color_overlay_rgb[1] = green;
    game_graphics_runtime.color_overlay_rgb[2] = blue;
}
