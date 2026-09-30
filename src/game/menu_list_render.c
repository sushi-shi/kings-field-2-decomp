#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

ADDRESS(0x8001fc94, 0xab4)
void func_8001fc94(const KfMenuRenderList *view, s32 render_mode)
{
    const KfMenuList *list = &view->list;
    const KfMenuSpriteDef *sprite;
    const s16 *row_glyphs = view->row_glyphs;
    const KfMenuLabelSuffix *detail_rows = view->detail_rows;
    const s32 *number_values = view->number_values;
    const u8 *byte_values = view->byte_values;
    KfMenuGlyphString text;
    s32 card_columns;
    s32 row;
    s32 code;
    s32 value;
    s32 y;

    if (list->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[5], &list->title.position);
        menu_draw_string(&menu_sprite_defs[1], &list->title);
    }

    row_glyphs += list->scroll_offset * list->glyphs_per_entry;
    detail_rows += list->scroll_offset;
    number_values += list->scroll_offset;
    byte_values += list->scroll_offset;

    if (list->visible_rows != 0 && list->entry_count != 0) {
        card_columns = (u32)(render_mode - 8) < 2;
        for (row = 0; row < list->entry_count && row < list->visible_rows; row++) {
            text.position.x = list->list_x + 5;
            text.position.y = list->list_y + 5 + row * 14;
            for (code = 0; code < list->glyphs_per_entry; code++)
                text.glyphs.codes[code] = *row_glyphs++;
            menu_draw_string(&menu_sprite_defs[1], &text);
    
            if (render_mode == 3) {
                text.position.x += 84;
                for (code = 0; code < 10; code++)
                    text.glyphs.codes[code] = detail_rows->codes[code];
                text.glyphs.codes[10] = -1;
                detail_rows++;
                menu_draw_string(&menu_sprite_defs[1], &text);
            }
    
            if ((u32)(render_mode - 10) < 6 && render_mode != 12) {
                text.position.x += 140;
                if (render_mode == 15)
                    menu_format_number(*number_values, 6, 0, 6, text.glyphs.codes);
                else
                    menu_format_number(*number_values, 6, 0, 2, text.glyphs.codes);
                number_values++;
                menu_draw_number(&menu_sprite_defs[0], &text);
            }
    
            if (render_mode == 2 || render_mode == 4) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
    
            if (card_columns) {
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
                && !card_columns && render_mode != 16) {
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

    y = 0;
    for (row = 0; row < list->visible_rows; row++) {
        sprite = row == list->cursor_row ? &menu_sprite_defs[10]
            : &menu_sprite_defs[8];
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

    if ((u32)(render_mode - 10) < 2 || (u32)(render_mode - 13) < 2)
        func_80020990(1);
    else if (render_mode == 15)
        func_80020990(3);
    if ((u32)(render_mode - 8) < 2)
        func_80021a60();
}
