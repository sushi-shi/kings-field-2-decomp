#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

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
