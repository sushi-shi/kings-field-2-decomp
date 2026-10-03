#include <kf/lib/address.h>
#include <kf/game/event_counter.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

enum { KF_PREVIEW_ANGLE_MASK = 0xfff };

ADDRESS(0x80020748, 0xf4)
void menu_draw_two_option(const KfMenuGlyphString *accept_label,
    const KfMenuGlyphString *decline_label, s32 selected_choice, s32 confirmation)
{
    if (selected_choice == KF_MENU_CHOICE_ACCEPT) {
        menu_blit_sprite(&menu_sprite_defs[KF_MENU_SPRITE_SELECTION_CURSOR],
            &accept_label->position);
    } else {
        menu_blit_sprite(&menu_sprite_defs[KF_MENU_SPRITE_SELECTION_CURSOR],
            &decline_label->position);
    }
    if (confirmation == KF_MENU_CONFIRM_REQUESTED) {
        if (selected_choice == KF_MENU_CHOICE_ACCEPT) {
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_HIGHLIGHT],
                &accept_label->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
                &decline_label->position);
        } else {
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
                &accept_label->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_HIGHLIGHT],
                &decline_label->position);
        }
    } else {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
            &accept_label->position);
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
            &decline_label->position);
    }
    menu_draw_string(&menu_sprite_defs[KF_MENU_SPRITE_GLYPH_ATLAS], accept_label);
    menu_draw_string(&menu_sprite_defs[KF_MENU_SPRITE_GLYPH_ATLAS], decline_label);
}

ADDRESS(0x8002083c, 0x154)
void menu_update_item_preview(s32 item_id)
{
    MATRIX rotation;
    MATRIX light;
    MATRIX lit;
    MATRIX color;

    if (player_state.unknown_c9[2] == 0 || (u8)item_id == 0xff) {
        return;
    }

    rotation.t[0] = menu_item_preview_translation.vx;
    rotation.t[1] = menu_item_preview_translation.vy;
    rotation.t[2] = menu_item_preview_translation.vz;
    menu_item_preview_rotation.vy += menu_item_preview_rotation_step;
    menu_item_preview_rotation.vx &= KF_PREVIEW_ANGLE_MASK;
    menu_item_preview_rotation.vy &= KF_PREVIEW_ANGLE_MASK;
    menu_item_preview_rotation.vz &= KF_PREVIEW_ANGLE_MASK;
    RotMatrix(&menu_item_preview_rotation, &rotation);

    light.m[0][0] = 0x800;
    light.m[0][1] = 0x800;
    light.m[0][2] = -0x800;
    light.m[1][0] = 0;
    light.m[1][1] = 0;
    light.m[1][2] = -0x800;
    light.m[2][0] = 0x1000;
    light.m[2][1] = 0;
    light.m[2][2] = -0x800;
    MulMatrix0(&light, &rotation, &lit);
    SetLightMatrix(&lit);

    color.m[0][0] = 0x1000;
    color.m[0][1] = 0x1000;
    color.m[0][2] = 0x1000;
    color.m[1][0] = 0x1000;
    color.m[1][1] = 0x1000;
    color.m[1][2] = 0x1000;
    color.m[2][0] = 0x1000;
    color.m[2][1] = 0x1000;
    color.m[2][2] = 0x1000;
    SetColorMatrix(&color);
    SetRotMatrix(&rotation);
    SetTransMatrix(&rotation);
    menu_render_item_model();
    current_poly_ft4 = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
}

ADDRESS(0x80020990, 0x1c0)
void menu_draw_status_counters(s32 kind)
{
    KfMenuGlyphString heading;
    KfMenuGlyphString amount;

    heading.position.x = 183;
    heading.position.y = 33;
    if (kind == 3) {
        heading.glyphs.codes[0] = 7;
        heading.glyphs.codes[1] = 41;
        heading.glyphs.codes[2] = 12;
        heading.glyphs.codes[3] = 15;
        heading.glyphs.codes[4] = 42;
        heading.glyphs.codes[5] = KF_MENU_TEXT_END;
    } else {
        heading.glyphs.codes[0] = 221;
        heading.glyphs.codes[1] = 222;
        heading.glyphs.codes[2] = 209;
        heading.glyphs.codes[3] = KF_MENU_TEXT_END;
    }
    menu_blit_sprite_translucent(&menu_sprite_defs[5], &heading.position);
    menu_draw_string(&menu_sprite_defs[1], &heading);

    amount.position.x = heading.position.x + 56;
    amount.position.y = heading.position.y;
    if (kind == 3)
        menu_format_number(game_counter_bytes[0x60], 7, 0, 6, amount.glyphs.codes);
    else
        menu_format_number(player_state.gold, 7, 0, 2, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    if (kind != 2) {
        heading.position.x = 183;
        heading.position.y = 61;
        heading.glyphs.codes[0] = 202;
        heading.glyphs.codes[1] = 203;
        heading.glyphs.codes[2] = KF_MENU_TEXT_END;
        menu_blit_sprite_translucent(&menu_sprite_defs[5], &heading.position);
        menu_draw_string(&menu_sprite_defs[1], &heading);
        amount.position.x = heading.position.x + 91;
        amount.position.y = heading.position.y;
        menu_format_number(menu_item_quantity, 2, 0, 1, amount.glyphs.codes);
        menu_draw_number(&menu_sprite_defs[0], &amount);
    }
}
