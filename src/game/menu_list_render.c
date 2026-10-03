#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

ADDRESS(0x8001fc94, 0xab4)
void func_8001fc94(const void *list_state, s32 render_mode)
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
