#include <kf/lib/address.h>
#include <kf/game/event_counter.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

DATA(0x80063e80, 0xf0)
KfMenuSpriteDef menu_sprite_defs[KF_MENU_SPRITE_COUNT] = {
    {0x1d, 0x7f64, 240, 0, 7, 14},
    {0x1d, 0x7f64, 0, 0, 15, 15},
    {0x1c, 0x7ee4, 128, 128, 14, 14},
    {0x1e, 0x7fa4, 160, 0, 54, 24},
    {0x1e, 0x7fa4, 160, 32, 54, 24},
    {0x1e, 0x7fa4, 0, 0, 124, 24},
    {0x1e, 0x7fa4, 0, 32, 124, 24},
    {0x1e, 0x7fa4, 0, 64, 236, 6},
    {0x1e, 0x7fa4, 0, 72, 236, 14},
    {0x1e, 0x7fa4, 0, 104, 236, 6},
    {0x1e, 0x7fa4, 0, 88, 236, 14},
    {0x1e, 0x7fa4, 0, 120, 33, 33},
    {0x1e, 0x7fa4, 40, 120, 28, 33},
    {0x1e, 0x7fa4, 72, 120, 33, 33},
    {0x1e, 0x7fa4, 0, 160, 33, 28},
    {0x1e, 0x7fa4, 40, 160, 28, 28},
    {0x1e, 0x7fa4, 72, 160, 33, 28},
    {0x1e, 0x7fa4, 0, 192, 33, 33},
    {0x1e, 0x7fa4, 40, 192, 28, 33},
    {0x1e, 0x7fa4, 72, 192, 33, 33},
};

DATA(0x80063f70, 0x9a0)
KfMenuWindowLayout menu_window_layouts[KF_MENU_WINDOW_COUNT] = {
    {
        {{0, 0}, {{-1}}},
        {
            {{31, 19}, {{0, 1, 18, 32, 109, 114, 66, -1}}},
            {{31, 45}, {{120, 121, 109, 114, 66, -1}}},
            {{31, 71}, {{112, 113, -1}}},
            {{31, 97}, {{137, 138, 57, 122, 208, -1}}},
            {{31, 123}, {{117, 82, 106, -1}}},
            {{31, 149}, {{11, 12, 18, 32, -1}}},
            {{31, 175}, {{4, 8219, 11, 56, 39, -1}}},
            {{31, 201}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{11, 12, 18, 32, -1}}},
        {
            {{31, 45}, {{44, 45, 4115, -1}}},
            {{31, 71}, {{4104, 45, 32, 109, 99, 97, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
            {{17, 19}, {{13, 45, 4123, -1}}},
            {{17, 19}, {{44, 45, 4115, -1}}},
            {{17, 19}, {{4104, 45, 32, 109, 99, 97, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{4, 8219, 11, 56, 39, -1}}},
        {
            {{31, 45}, {{213, 214, 215, -1}}},
            {{31, 71}, {{215, 216, -1}}},
            {{31, 97}, {{133, 134, 217, 218, -1}}},
            {{31, 123}, {{219, 220, 217, 218, -1}}},
            {{31, 149}, {{0, 1, 18, 32, 217, 218, -1}}},
            {{31, 175}, {{238, 239, 213, 214, -1}}},
            {{31, 201}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{116, 66, 57, 115, 106, -1}}},
        {
            {{31, 45}, {{116, 66, -1}}},
            {{31, 71}, {{115, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{164, 206, -1}}},
        {
            {{31, 45}, {{116, 66, -1}}},
            {{31, 71}, {{256, 257, 109, 207, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{0, -1}}},
        {
            {{31, 45}, {{258, 259, -1}}},
            {{31, 71}, {{141, 142, 206, -1}}},
        },
    },
    {
        {{0, 0}, {{-1}}},
        {
            {{98, 94}, {{89, 4171, 97, 106, -1}}},
            {{98, 120}, {{44, 45, 4115, 76, 106, -1}}},
        },
    },
    {0},
};

ADDRESS(0x8001fb8c, 0x108)
void menu_draw_window(s32 window_kind, s32 count, s32 highlight, s32 confirmation)
{
    const KfMenuWindowLayout *layout = &menu_window_layouts[window_kind];
    const KfMenuGlyphString *row = &layout->rows[0];
    s32 index;

    if (layout->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[5], &layout->title.position);
        menu_draw_string(&menu_sprite_defs[1], &layout->title);
    }
    if (count > 0) {
        index = 0;
        do {
            if (index == highlight && confirmation == KF_MENU_CONFIRM_REQUESTED)
                menu_blit_sprite_translucent(&menu_sprite_defs[6], &row->position);
            else
                menu_blit_sprite_translucent(&menu_sprite_defs[5], &row->position);
            if (index == highlight)
                menu_blit_sprite(&menu_sprite_defs[2], &row->position);
            menu_draw_string(&menu_sprite_defs[1], row);
            row++;
            index++;
        } while (index < count);
    }
}
ADDRESS(0x8001fc94, 0xab4)
void menu_render_list(const void *list_state, s32 render_mode)
{
    const KfMenuRenderList *view = list_state;
    const KfMenuList *list = &view->list;
    const KfMenuSpriteDef *sprite;
    const s16 *row_glyphs = view->row_glyphs;
    const s16 *detail_codes = (const s16 *)view->detail_rows;
    const s32 *number_values = view->number_values;
    const u8 *byte_values = view->byte_values;
    KfMenuGlyphString text;
    s16 *out_codes;
    s32 row;
    s32 code;
    s32 value;
    s32 y;

    if (list->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[5], &list->title.position);
        menu_draw_string(&menu_sprite_defs[1], &list->title);
    }

    detail_codes += list->scroll_offset * 10;
    row_glyphs += list->scroll_offset * list->glyphs_per_entry;
    number_values += list->scroll_offset;
    byte_values += list->scroll_offset;

    {
        for (row = 0; row < list->visible_rows && row < list->entry_count; row++) {
            text.position.x = list->list_x + 5;
            text.position.y = list->list_y + 5 + row * 14;
            out_codes = text.glyphs.codes;
            for (code = 0; code < list->glyphs_per_entry; code++)
                *out_codes++ = *row_glyphs++;
            menu_draw_string(&menu_sprite_defs[1], &text);

            if (render_mode == 3) {
                text.position.x += 84;
                out_codes = text.glyphs.codes;
                for (code = 0; code < 10; code++)
                    *out_codes++ = *detail_codes++;
                *out_codes = -1;
                menu_draw_string(&menu_sprite_defs[1], &text);
            }

            if (((u32)render_mode - 10u) < 6u && render_mode != 12) {
                value = *number_values;
                text.position.x += 140;
                if (render_mode == 15)
                    menu_format_number(value, 6, 0, 6, text.glyphs.codes);
                else
                    menu_format_number(value, 6, 0, 2, text.glyphs.codes);
                menu_draw_number(&menu_sprite_defs[0], &text);
                number_values++;
            }

            if (render_mode == 2 || render_mode == 4) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }

            if (((u32)render_mode - 8u) < 2u) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 98;
                    menu_format_number(value, 7, 0, 4, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x += 91;
                    menu_format_number(value, 3, 0, 5, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }

            if (render_mode != 2 && render_mode != 3 && render_mode != 4
                && ((u32)render_mode - 8u) >= 2u && render_mode != 16) {
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x = list->list_x + 208;
                    menu_format_number(value, 2, 0, 1, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
            if (render_mode == 16) {
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x += 203;
                    menu_format_number(value, 2, 0, 1, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
        }
    }

    sprite = &menu_sprite_defs[7];
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 255, 255, 255);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4, list->list_x, list->list_y,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v,
        sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);

    row = 0;
    if (row < list->visible_rows) {
        y = 0;
        do {
            sprite = row == list->cursor_row ? &menu_sprite_defs[10]
                : &menu_sprite_defs[8];
            row++;
            primitive_buffer_begin_poly_ft4();
            setRGB0(current_poly_ft4, 255, 255, 255);
            current_poly_ft4->tpage = sprite->tpage;
            current_poly_ft4->clut = sprite->clut;
            setXYWH(current_poly_ft4, list->list_x, list->list_y + y + 5,
                sprite->width, sprite->height);
            setUVWH(current_poly_ft4, sprite->u, sprite->v,
                sprite->width, sprite->height);
            SetSemiTrans((void *)current_poly_ft4, 1);
            primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
            y += 14;
        } while (row < list->visible_rows);
    }

    sprite = &menu_sprite_defs[9];
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 255, 255, 255);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4, list->list_x,
        list->list_y + list->visible_rows * 14 + 5,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v,
        sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);

    if (((u32)render_mode - 10u) < 2u || ((u32)render_mode - 13u) < 2u)
        menu_draw_status_counters(1);
    else if (render_mode == 15)
        menu_draw_status_counters(3);
    if (((u32)render_mode - 8u) < 2u)
        func_80021a60();
}
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

    if (player_state.item_preview_enabled == 0 || (u8)item_id == 0xff) {
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
ADDRESS(0x80020b50, 0x1d0)
void menu_blit_sprite_translucent(
    const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 0xff, 0xff, 0xff);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4,
        position->x - KF_MENU_TRANSLUCENT_SPRITE_OFFSET,
        position->y - KF_MENU_TRANSLUCENT_SPRITE_OFFSET,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v, sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}

enum {
    KF_MENU_CURSOR_COLOR = 0x40,
    KF_MENU_CURSOR_LEFT_MARGIN = 8,
    KF_MENU_CURSOR_TEXTURE_PAGE_BASE = 500,
    KF_MENU_CURSOR_CLUT_BASE = 36
};

ADDRESS(0x80020d20, 0x1d8)
void menu_blit_sprite(const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    s32 frame = menu_cursor_animation_frame;

    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4,
        KF_MENU_CURSOR_COLOR, KF_MENU_CURSOR_COLOR, KF_MENU_CURSOR_COLOR);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = ((frame + KF_MENU_CURSOR_TEXTURE_PAGE_BASE) << 6)
        + KF_MENU_CURSOR_CLUT_BASE;
    setXYWH(current_poly_ft4,
        position->x - (sprite->width + KF_MENU_CURSOR_LEFT_MARGIN),
        position->y, sprite->width, sprite->height);
    setUVWH(current_poly_ft4,
        sprite->u + (frame << 4), sprite->v, sprite->width, sprite->height);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}

enum {
    KF_MENU_FIXED_CURSOR_COLOR = 0x38,
    KF_MENU_FIXED_CURSOR_CLUT = 0x7d24,
    KF_MENU_FIXED_CURSOR_LEFT_MARGIN = 7
};

ADDRESS(0x80020ef8, 0x1b4)
void menu_blit_sprite_fixed_clut(
    const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4,
        KF_MENU_FIXED_CURSOR_COLOR,
        KF_MENU_FIXED_CURSOR_COLOR,
        KF_MENU_FIXED_CURSOR_COLOR);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = KF_MENU_FIXED_CURSOR_CLUT;
    setXYWH(current_poly_ft4,
        position->x - (sprite->width + KF_MENU_FIXED_CURSOR_LEFT_MARGIN),
        position->y, sprite->width, sprite->height);
    setUVWH(current_poly_ft4,
        sprite->u, sprite->v, sprite->width, sprite->height);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}
static inline void menu_begin_text_glyph(
    const KfMenuSpriteDef *font, const KfMenuGlyphString *string, s32 x_offset)
{
    primitive_buffer_begin_poly_ft4();
    current_poly_ft4->tpage = font->tpage;
    current_poly_ft4->clut = font->clut;
    setXYWH(current_poly_ft4, string->position.x + x_offset,
        string->position.y, font->width, font->height);
}

ADDRESS(0x800210ac, 0x464)
void menu_draw_string(const KfMenuSpriteDef *font, const KfMenuGlyphString *string)
{
    const s16 *code = string->glyphs.codes;
    s32 i;
    s32 x_offset;

    for (i = 0; *code != KF_MENU_TEXT_END; code++, i++) {
        u32 glyph;

        x_offset = i * KF_MENU_GLYPH_ADVANCE;
        menu_begin_text_glyph(font, string, x_offset);
        glyph = *code & KF_MENU_TEXT_GLYPH_MASK;
        setUVWH(current_poly_ft4,
            (glyph % KF_MENU_FONT_COLUMNS) * KF_MENU_FONT_CELL_WIDTH,
            (glyph / KF_MENU_FONT_COLUMNS) * KF_MENU_FONT_CELL_HEIGHT,
            font->width, font->height);
        primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);

        if (*code & KF_MENU_TEXT_DAKUTEN) {
            menu_begin_text_glyph(font, string, x_offset);
            setUVWH(current_poly_ft4, KF_MENU_DAKUTEN_U, KF_MENU_KANA_MARK_V,
                font->width, font->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        }

        if (*code & KF_MENU_TEXT_HANDAKUTEN) {
            menu_begin_text_glyph(font, string, x_offset);
            setUVWH(current_poly_ft4, KF_MENU_HANDAKUTEN_U, KF_MENU_KANA_MARK_V,
                font->width, font->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        }
    }
}

ADDRESS(0x80021510, 0x2e0)
void menu_draw_number(const KfMenuSpriteDef *font, const KfMenuGlyphString *string)
{
    const s16 *code;
    s32 x_offset;

    code = string->glyphs.codes;
    if (*code == KF_MENU_TEXT_END) {
        return;
    }
    x_offset = 0;
    do {
        primitive_buffer_begin_poly_ft4();
        current_poly_ft4->tpage = font->tpage;
        current_poly_ft4->clut = font->clut;
        setXYWH(current_poly_ft4,
            string->position.x + x_offset, string->position.y,
            font->width, font->height);
        if (*code < KF_MENU_NUMBER_COLUMN_ROWS) {
            setUVWH(current_poly_ft4,
                font->u, *code * KF_MENU_FONT_CELL_HEIGHT,
                font->width, font->height);
        } else {
            setUVWH(current_poly_ft4,
                (u8)(font->u + KF_MENU_NUMBER_COLUMN_WIDTH),
                (*code - KF_MENU_NUMBER_COLUMN_ROWS) * KF_MENU_FONT_CELL_HEIGHT,
                font->width, font->height);
        }
        primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        code++;
        x_offset += KF_MENU_DIGIT_ADVANCE;
    } while (*code != KF_MENU_TEXT_END);
}
ADDRESS(0x800217f0, 0x270)
void menu_draw_nine_slice_panel(s32 x, s32 y, s32 width, s32 height,
    s32 overlap_x, s32 overlap_y)
{
    const KfMenuSpriteDef *tile;
    u16 clut;
    s32 row;
    s32 column;
    s32 tile_index;
    s32 draw_x;
    s32 draw_y;
    s32 draw_width;
    s32 draw_height;

    width -= 94;
    height -= 94;
    tile_index = 9;
    tile = &menu_sprite_defs[11];
    draw_y = y;
    for (row = 0; row < 3; row++) {
        draw_height = tile->height;
        if (row == 1) {
            draw_y += 33;
            draw_height += height + overlap_y;
        } else if (row == 2) {
            draw_y += 28 + height;
        }
        draw_x = x;
        for (column = 0; column < 3; column++) {
            draw_width = tile->width;
            if (column == 1) {
                draw_x += 33;
                draw_width += width + overlap_x;
            } else if (column == 2) {
                draw_x += 28 + width;
            }
            primitive_buffer_begin_poly_ft4();
            setRGB0(current_poly_ft4, 0xff, 0xff, 0xff);
            SetSemiTrans((void *)current_poly_ft4, 1);
            current_poly_ft4->tpage = tile->tpage;
            clut = tile->clut;
            setXYWH(current_poly_ft4, draw_x, draw_y, draw_width, draw_height);
            current_poly_ft4->clut = clut;
            setUVWH(current_poly_ft4, tile->u, tile->v, tile->width, tile->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
            tile_index++;
            tile = &menu_sprite_defs[tile_index + 2];
        }
    }
}
