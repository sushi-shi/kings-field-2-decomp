#include <kf/lib/address.h>
#include <kf/game/menu.h>

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
